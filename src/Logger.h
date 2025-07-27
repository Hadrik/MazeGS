//
// Created by trric on 7/20/2025.
//

#ifndef LOGGER_H
#define LOGGER_H
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>


class Logger {
public:
    enum class Target {
        None = 0,
        Cout,
        Window,
    };

    static Logger& get();


    template<typename T>
    Logger& operator<< (const T& value) {
        if (_target != Target::None) {
            _buffer << value;
            if (_target == Target::Cout) {
                std::cout << value;
            }
        }
        return *this;
    }
    Logger& operator<< (std::ostream& (*manip)(std::ostream&));

    void setTarget(Target target);
    std::string getTargetName() const;
    static std::string getTargetName(Target t);
    static std::vector<std::tuple<Target, std::string>> getTargetOptions();
    void drawWindow();
    void clear();

private:
    Logger() = default;
    ~Logger() = default;

    void flushBuffer();

    Target _target = Target::Cout;
    std::ostringstream _buffer;
    std::vector<std::string> _lines;
};



#endif //LOGGER_H
