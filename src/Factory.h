//
// Created by trric on 7/21/2025.
//

#ifndef FACTORY_H
#define FACTORY_H
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "ICellPicker.h"


template<class C>
class Factory {
public:
    Factory(const Factory&) = delete;
    Factory& operator=(const Factory&) = delete;

    static Factory& instance() {
        static Factory instance;
        return instance;
    }

    void setCellPicker(ICellPicker* cellPicker) {
        _cellPicker = cellPicker;
    }

    template<class T>
    void registerClass(const std::string& name) {
        _creators[name] = [this]() { return createInstance<T>(); };
    }

    std::unique_ptr<C> create(const std::string& name) {
        auto it = _creators.find(name);
        if (it == _creators.end()) return nullptr;
        return it->second();
    }

    [[nodiscard]] std::vector<std::string> getRegisteredNames() const {
        std::vector<std::string> names;
        for (const auto& pair : _creators) {
            names.push_back(pair.first);
        }
        return names;
    }

private:
    Factory() = default;

    template<class T>
    std::unique_ptr<C> createInstance() {
        if constexpr (std::is_constructible_v<T, ICellPicker*>) {
            if (!_cellPicker) return nullptr;
            return std::make_unique<T>(_cellPicker);
        } else {
            return std::make_unique<T>();
        }
    }

    std::map<std::string, std::function<std::unique_ptr<C>()>> _creators;
    ICellPicker* _cellPicker = nullptr;
};


template<class B, class T>
class FactoryRegistrar {
public:
    explicit FactoryRegistrar() {
        Factory<B>::instance().template registerClass<T>(T::getRegisterName());
    }
};


#endif //FACTORY_H
