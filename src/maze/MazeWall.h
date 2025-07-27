//
// Created by trric on 7/17/2025.
//

#ifndef MAZEWALL_H
#define MAZEWALL_H

#include <mutex>

#include "raylib-cpp.hpp"

class MazeWall {
public:
    MazeWall();
    MazeWall(const MazeWall& other);
    explicit MazeWall(bool exists);
    ~MazeWall() = default;

    void set(bool exists);
    MazeWall& operator= (bool exists);
    [[nodiscard]] bool get() const;
    explicit operator bool() const;

    [[nodiscard]] const raylib::Color& getTopColor() const;
    void setTopColor(const raylib::Color& color);
    void setTopColor(const Color& color);

private:
    bool _exists;
    std::vector<raylib::Color> _colors = {};

    mutable std::mutex _mutex_exists;
    mutable std::mutex _mutex_colors;
};



#endif //MAZEWALL_H
