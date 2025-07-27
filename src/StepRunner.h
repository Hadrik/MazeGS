//
// Created by trric on 7/24/2025.
//

#ifndef STEPRUNNER_H
#define STEPRUNNER_H
#include <functional>
#include <memory>
#include <thread>

#include "Logger.h"
#include "maze/Maze.h"


class StepRunner {
public:
    using StepFunction = std::function<bool()>;
    using BeginFunction = std::function<void(std::unique_ptr<Maze>&)>;
    using CleanupFunction = std::function<void()>;

    StepRunner() : _log(Logger::get()) {}
    ~StepRunner();

    void run(const BeginFunction& begin, StepFunction step, CleanupFunction clean, std::unique_ptr<Maze>& maze);
    void stop();
    void drawGui();
    [[nodiscard]] bool isRunning() const;

private:
    void worker();

    enum class Mode {
        Manual,
        Automatic
    };

    std::thread _thread;
    std::atomic<bool> _isRunning = false;
    std::atomic<bool> _stepRequest = false;

    StepFunction _stepFunction;
    CleanupFunction _cleanupFunction;

    Mode _mode = Mode::Automatic;
    int _delay = 50;

    Logger& _log;
};



#endif //STEPRUNNER_H
