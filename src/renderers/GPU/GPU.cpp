//
// Created by trric on 7/30/2025.
//

#include "GPU.h"

#include <fstream>
#include <filesystem>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "GLFW/glfw3.h"

namespace {
    FactoryRegistrar<IRenderer, GPU> registrar;
}

GPU::GPU() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    _window = glfwCreateWindow(1200, 800, "MazeGS", nullptr, nullptr);
    glfwMakeContextCurrent(_window);
    glfwSwapInterval(1);
    gladLoadGL(glfwGetProcAddress);
    glViewport(0, 0, 1200, 800);
    glfwSetFramebufferSizeCallback(_window, [](GLFWwindow*, const int width, const int height) {
        glViewport(0, 0, width, height);
    });

    ImGui::CreateContext();
    auto& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.ConfigWindowsMoveFromTitleBarOnly = true;
    io.ConfigDebugIsDebuggerPresent = true;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(_window, true);
    ImGui_ImplOpenGL3_Init();

    vao = vbo = ebo = ivbo = sid = fbo = rbo = tcb = tid = 0;

    loadShader("CellShader.vert", "CellShader.frag");

    glGenTextures(1, &tid);
    glBindTexture(GL_TEXTURE_2D_ARRAY, tid);
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, 100, 100, 5, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    const auto imgs = loadSprites();
    for (size_t i = 0; i < imgs.size(); i++) {
        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, static_cast<int>(i), 100, 100, 1, GL_RGBA, GL_UNSIGNED_BYTE, imgs.at(i).GetData());
    }
    glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
    glUniform1i(glGetUniformLocation(sid, "spriteSheet"), 0);

    const float verts[] = {
        1.0f, 0.0f,   1.0f, 0.0f,
        1.0f, -1.0f,  1.0f, 1.0f,
        0.0f, -1.0f,  0.0f, 1.0f,
        0.0f, 0.0f,   0.0f, 0.0f,
    };
    const unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    glGenBuffers(1, &ivbo);

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), &verts, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void *>(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    glEnableVertexAttribArray(5);
    glEnableVertexAttribArray(6);
    glBindBuffer(GL_ARRAY_BUFFER, ivbo);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(InstanceData), nullptr);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(InstanceData), reinterpret_cast<void *>(2 * sizeof(float)));
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(InstanceData), reinterpret_cast<void *>(4 * sizeof(float)));
    glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(InstanceData), reinterpret_cast<void *>(7 * sizeof(float)));
    glVertexAttribIPointer(6, 1, GL_INT, sizeof(InstanceData), reinterpret_cast<void *>(8 * sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glGenTextures(1, &tcb);
    glBindTexture(GL_TEXTURE_2D, tcb);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tcb, 0);

    glGenFramebuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GPU::~GPU() {
    glDeleteFramebuffers(1, &fbo);
    glDeleteTextures(1, &tcb);
    glDeleteRenderbuffers(1, &rbo);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &vbo);
    glDeleteProgram(sid);
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(_window);
    glfwTerminate();
}

void GPU::newFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::DockSpaceOverViewport(0, nullptr, ImGuiDockNodeFlags_PassthruCentralNode);
}

void GPU::render(const Maze *maze) {
    glfwPollEvents();

    if (maze) {
        resizeFramebuffer(Vec2(maze->getWidth(), maze->getHeight()));

        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glViewport(0, 0, _textureWidth, _textureHeight);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const int instances = generateInstanceBufferData(maze);
        glUseProgram(sid);
        glBindVertexArray(vao);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D_ARRAY, tid);
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr, instances);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    int dw, dh;
    glfwGetFramebufferSize(_window, &dw, &dh);
    glViewport(0, 0, dw, dh);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui::Begin("Maze view");
    {
        if (maze) {
            ImVec2 availSize = ImGui::GetContentRegionAvail();
            ImVec2 imageSize = availSize;
            float textureAspectRatio = static_cast<float>(_textureWidth) / static_cast<float>(_textureHeight);
            float availAspectRatio = availSize.x / availSize.y;

            if (textureAspectRatio > availAspectRatio) {
                imageSize.y = availSize.x / textureAspectRatio;
            } else {
                imageSize.x = availSize.y * textureAspectRatio;
            }

            ImVec2 cPos = ImGui::GetCursorPos();
            cPos.x += (availSize.x - imageSize.x) / 2;
            cPos.y += (availSize.y - imageSize.y) / 2;
            ImGui::SetCursorPos(cPos);

            ImGui::Image(tcb, imageSize, {0, 1}, {1, 0});
        } else {
            ImGui::Text("No maze generated");
        }
    }
    ImGui::End();

    Logger::get().drawWindow();

    if (_isPickerActive && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
        handleCellPicking(maze);
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow* backup_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_context);
    }
    glfwSwapBuffers(_window);
}

bool GPU::shouldExit() {
    return glfwWindowShouldClose(_window);
}

void GPU::resizeFramebuffer(const Vec2 mazeSize) {
    const int newWidth = mazeSize.width * CELL_SIZE;
    const int newHeight = mazeSize.height * CELL_SIZE;

    if (newWidth > 0 && newHeight > 0 && (_textureHeight != newHeight || _textureWidth != newWidth )) {
        _textureHeight = newHeight;
        _textureWidth = newWidth;

        glBindTexture(GL_TEXTURE_2D, tcb);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, newWidth, newHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, newWidth, newHeight);

        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            Logger::get() << "Framebuffer not complete" << std::endl;
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}

int GPU::generateInstanceBufferData(const Maze *maze) const {
    const size_t width = maze->getWidth();
    const size_t height = maze->getHeight();

    std::vector<InstanceData> data;
    data.reserve(width * height);

    const InstanceData::CellScale scale = {
        .x = 2.0f / width,
        .y = 2.0f / height
    };

    for (size_t col = 0; col < width; col++) {
        for (size_t row = 0; row < height; row++) {
            const auto& cell = maze->getCells().at(col).at(row);
            InstanceData i {};

            i.vertOffset = InstanceData::VertOffset {
                .x = -1.0f + col * scale.x,
                .y = 1.0f - row * scale.y
            };
            i.cellScale = scale;

            const auto& c = cell.infill().topColor();
            i.color = InstanceData::Color {
                .r = c.r / 255.0f,
                .g = c.g / 255.0f,
                .b = c.b / 255.0f
            };
            i.spriteRotation = mapPrimitiveToSpriteRotation(cell.infill().topPrimitive().shape);
            i.spriteOffset = mapPrimitiveToSpriteLocation(cell.infill().topPrimitive().shape);

            data.emplace_back(i);
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, ivbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(InstanceData) * data.size(), data.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return width * height;
}

void GPU::renderWalls() {
    // for (const auto& wall : _walls) {
    //     DrawLineEx(wall.start, wall.end, wall.thickness, wall.color);
    // }
}

void GPU::handleCellPicking(const Maze *maze) {
    // TODO: Broken

    ImVec2 mousePos = ImGui::GetMousePos();

    ImVec2 contentPos = ImGui::GetWindowContentRegionMin();
    ImVec2 windowPos = ImGui::GetWindowPos();
    mousePos.x -= contentPos.x + windowPos.x;
    mousePos.y -= contentPos.y + windowPos.y;

    auto col = static_cast<size_t>(mousePos.x / CELL_SIZE);
    auto row = static_cast<size_t>(mousePos.y / CELL_SIZE);

    if (row < maze->getHeight() && col < maze->getWidth()) {
        if (_pickerCallback) {
            _pickerCallback({col, row});
        }
        _isPickerActive = false;
        _pickerCallback = nullptr;
    }
}

void GPU::loadShader(const char *v, const char *f) {
    std::filesystem::path base = SRC_DIR;
    std::filesystem::path vert_path = base / v;
    std::filesystem::path frag_path = base / f;

    std::string vert_src;
    std::string frag_src;
    std::ifstream vert_file;
    std::ifstream frag_file;

    vert_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    frag_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        vert_file.open(vert_path);
        frag_file.open(frag_path);
        std::stringstream vss, fss;
        vss << vert_file.rdbuf();
        fss << frag_file.rdbuf();
        vert_file.close();
        frag_file.close();
        vert_src = vss.str();
        frag_src = fss.str();
    } catch (std::ifstream::failure& e) {
        std::cerr << "Fail reading shader file!\n" << e.what() << std::endl;
    }

    const char* vert_code = vert_src.c_str();
    const char* frag_code = frag_src.c_str();
    unsigned int vert_id, frag_id;
    int success;
    char info[512];

    vert_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_id, 1, &vert_code, nullptr);
    glCompileShader(vert_id);
    glGetShaderiv(vert_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vert_id, sizeof(info), nullptr, info);
        std::cerr << "Fail compiling vertex shader!\n" << info << std::endl;
    }

    frag_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_id, 1, &frag_code, nullptr);
    glCompileShader(frag_id);
    glGetShaderiv(frag_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(frag_id, sizeof(info), nullptr, info);
        std::cerr << "Fail compiling fragment shader!\n" << info << std::endl;
    }

    const unsigned int program = glCreateProgram();
    glAttachShader(program, vert_id);
    glAttachShader(program, frag_id);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, sizeof(info), nullptr, info);
        std::cerr << "Fail linking shader program!\n" << info << std::endl;
    }

    glDeleteShader(vert_id);
    glDeleteShader(frag_id);

    sid = program;
}

std::vector<RImage> GPU::loadSprites() const {
    std::filesystem::path base = SRC_DIR;
    const char* names[] = {
        "Arrow.png",
        "Circle.png",
        "Cross.png",
        "Square.png",
        "Triangle.png",
    };

    std::vector<RImage> result;
    result.reserve(5);

    for (const auto name : names) {
        std::filesystem::path path = base / name;

        result.emplace_back(path.string());
    }

    return result;
}

int GPU::mapPrimitiveToSpriteLocation(const PrimitiveShape shape) {
    switch (shape) {
        case PrimitiveShape::SQUARE_SMALL:
        case PrimitiveShape::SQUARE_LARGE: return 3;
        case PrimitiveShape::TRIANGLE_SMALL:
        case PrimitiveShape::TRIANGLE_LARGE: return 4;
        case PrimitiveShape::CIRCLE_SMALL:
        case PrimitiveShape::CIRCLE_LARGE: return 1;
        case PrimitiveShape::CROSS_SMALL:
        case PrimitiveShape::CROSS_LARGE: return 2;
        case PrimitiveShape::ARROW_UP:
        case PrimitiveShape::ARROW_RIGHT:
        case PrimitiveShape::ARROW_DOWN:
        case PrimitiveShape::ARROW_LEFT: return 0;
        default: return -1;
    }
}

float GPU::mapPrimitiveToSpriteRotation(const PrimitiveShape shape) {
    switch (shape) {
        case PrimitiveShape::ARROW_RIGHT: return 90 * DEG2RAD;
        case PrimitiveShape::ARROW_DOWN: return 180 * DEG2RAD;
        case PrimitiveShape::ARROW_LEFT: return 270 * DEG2RAD;
        default: return 0;
    }
}
