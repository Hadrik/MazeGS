//
// Created by trric on 7/30/2025.
//

#ifndef GPU_H
#define GPU_H
#include <external/glad.h>
#include <GLFW/glfw3.h>

#include "../IRenderer.h"


class GPU final : public IRenderer {
public:
    GPU();
    ~GPU() override;

    void newFrame() override;
    void render(const Maze *maze) override;

    bool shouldExit() override;

    void startPicker(const PickCallback cb) override { _isPickerActive = true; _pickerCallback = cb; }
    [[nodiscard]] bool isPickerActive() override { return _isPickerActive; }

    [[nodiscard]] const std::string & getName() const override { return getRegisterName(); }
    [[nodiscard]] static const std::string& getRegisterName() {
        static std::string registerName("GPU");
        return registerName;
    }

private:
    void resizeFramebuffer(Vec2 mazeSize);
    void renderWalls();
    static int mapPrimitiveToSpriteLocation(PrimitiveShape shape);
    void handleCellPicking(const Maze *maze);
    void loadShader(const char* v, const char* f);

    const unsigned int CELL_SIZE = 100;
    GLFWwindow* _window;

    int _textureWidth, _textureHeight;
    unsigned int vao, vbo, ebo, ivbo, sid, fbo, rbo, tcb;

    bool _isPickerActive = false;
    PickCallback _pickerCallback;
};


#endif //GPU_H
