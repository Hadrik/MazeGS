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
    struct CellInstanceData {
        struct VertOffset {
            float x, y;
        } vertOffset;
        struct CellScale {
            float x, y;
        } cellScale;

        struct Color {
            float r, g, b;
        } color;
        float spriteRotation;
        float spriteScale;
        int spriteOffset;
    };

    struct WallInstanceData {
        struct StartPoint {
            float x, y;
        } startPoint;
        struct EndPoint {
            float x, y;
        } endPoint;
        float thickness;
        struct Color {
            float r, g, b;
        } color;
    };

    void resizeFramebuffer(Vec2 mazeSize);
    int generateCellBufferData(const Maze* maze) const;
    int generateWallBufferData(const Maze* maze) const;
    static int mapPrimitiveToSpriteLocation(const Primitive& shape);
    static float mapPrimitiveToSpriteRotation(const Primitive& shape);
    static float mapPrimitiveToSpriteScale(const Primitive& shape);
    void handleCellPicking(const Maze *maze);
    unsigned int loadShader(const char* v, const char* f);
    std::vector<RImage> loadSprites() const;

    const unsigned int CELL_SIZE = 100;
    GLFWwindow* _window;

    int _textureWidth, _textureHeight;
    unsigned int cvao, cvbo, cebo, civbo, csid, wvao, wvbo, webo, wivbo, wsid, fbo, rbo, tcb, tid;

    bool _isPickerActive = false;
    PickCallback _pickerCallback;
};


#endif //GPU_H
