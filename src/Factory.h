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


template<class C>
class Factory {
public:
    Factory(const Factory&) = delete;
    Factory& operator=(const Factory&) = delete;

    static Factory& instance() {
        static Factory instance;
        return instance;
    }

    template<class T>
    void registerClass(const std::string& name) {
        _creators[name] = []() { return std::make_unique<T>(); };
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

    std::map<std::string, std::function<std::unique_ptr<C>()>> _creators;
};


template<class B, class T>
class FactoryRegistrar {
public:
    explicit FactoryRegistrar() {
        Factory<B>::instance().template registerClass<T>(T::getRegisterName());
    }
};


#endif //FACTORY_H
