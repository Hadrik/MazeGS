//
// Created by trric on 7/19/2025.
//

#include "Infill.h"

#include <algorithm>
#include <ranges>

Infill::Infill(const Infill &other) {
    _stack = other._stack;
}

Infill::Infill(Infill &&other) noexcept {
    _stack = std::move(other._stack);
}

Infill & Infill::operator=(Infill &&other) noexcept {
    if (this != &other) {
        _stack = std::move(other._stack);
    }
    return *this;
}

void Infill::push(const raylib::Color &color) {
    std::lock_guard lock(_mutex);
    _stack.emplace_back(color);
}

void Infill::push(const Primitive& primitive) {
    std::lock_guard lock(_mutex);
    _stack.emplace_back(primitive);
}

bool Infill::pop() {
    std::lock_guard lock(_mutex);
    if (_stack.empty()) return false;
    _stack.pop_back();
    return true;
}

bool Infill::pop(const ContainedType type) {
    std::lock_guard lock(_mutex);
    if (_stack.empty()) return false;
    const auto& it = std::find_if(_stack.rbegin(), _stack.rend(),
        [&](const std::variant<raylib::Color, Primitive>& var) {
        return (type == ContainedType::Color && std::holds_alternative<raylib::Color>(var))
            || (type == ContainedType::Primitive && std::holds_alternative<Primitive>(var));
    });
    if (it == _stack.rend()) return false;
    _stack.erase(std::next(it).base());
    return true;
}

const std::variant<raylib::Color, Primitive> & Infill::top() const {
    std::lock_guard lock(_mutex);
    return _stack.back();
}

const raylib::Color& Infill::topColor() const {
    std::lock_guard lock(_mutex);
    const auto& it = std::find_if(_stack.rbegin(), _stack.rend(),
        [&](const std::variant<raylib::Color, Primitive>& var) {
        return std::holds_alternative<raylib::Color>(var);
    });
    static const raylib::Color empty_color = raylib::Color::White();
    if (it == _stack.rend()) return empty_color;
    return std::get<raylib::Color>(*it);
}

const Primitive& Infill::topPrimitive() const {
    std::lock_guard lock(_mutex);
    const auto it = std::find_if(_stack.rbegin(), _stack.rend(),
        [&](const std::variant<raylib::Color, Primitive>& var) {
        return std::holds_alternative<Primitive>(var);
    });
    static const auto empty_primitive = Primitive();
    if (it == _stack.rend()) return empty_primitive;
    return std::get<Primitive>(*it);
}

std::pair<raylib::Color, Primitive> Infill::topPair() const {
    return std::make_pair(topColor(), topPrimitive());
}
