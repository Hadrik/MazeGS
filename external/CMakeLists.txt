include(FetchContent)

FetchContent_Declare(raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.5
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)
FetchContent_MakeAvailable(raylib)

FetchContent_Declare(raylib-cpp
        GIT_REPOSITORY https://github.com/RobLoach/raylib-cpp.git
        GIT_TAG v5.5.0
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)
set(BUILD_RAYLIB_CPP_EXAMPLES OFF CACHE INTERNAL "")
FetchContent_MakeAvailable(raylib-cpp)

FetchContent_Declare(raylib-imgui
        GIT_REPOSITORY https://github.com/raylib-extras/rlImGui.git
        GIT_TAG main
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)
FetchContent_MakeAvailable(raylib-imgui)
FetchContent_GetProperties(raylib-imgui SOURCE_DIR RLIMGUI_DIR)
add_library(rlImGui STATIC ${RLIMGUI_DIR}/rlImGui.cpp)
target_include_directories(rlImGui PUBLIC ${RLIMGUI_DIR})
target_link_libraries(rlImGui PUBLIC raylib imgui)

FetchContent_Declare(ImGui
        GIT_REPOSITORY https://github.com/ocornut/imgui
        GIT_TAG v1.92.1-docking
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)
FetchContent_MakeAvailable(ImGui)
FetchContent_GetProperties(ImGui SOURCE_DIR IMGUI_DIR)
set(IMGUI_BACKENDS_DIR "${IMGUI_DIR}/backends")

add_library(imgui STATIC
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_demo.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
)

target_include_directories(imgui PUBLIC
        ${IMGUI_DIR}
)

#target_link_libraries(imgui PUBLIC raylib-imgui)