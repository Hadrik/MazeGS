//
// Created by trric on 7/22/2025.
//

#ifndef GENERATORBASE_H
#define GENERATORBASE_H
#include "IGenerator.h"


template<class T>
class GeneratorBase : public IGenerator {
public:
    [[nodiscard]] const std::string& getName() const override {
        return T::getRegisterName();
    }
};



#endif //GENERATORBASE_H
