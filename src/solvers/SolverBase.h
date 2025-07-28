//
// Created by trric on 7/28/2025.
//

#ifndef SOLVERBASE_H
#define SOLVERBASE_H
#include "ISolver.h"


template<class T>
class SolverBase : public ISolver {
public:
    [[nodiscard]] const std::string& getName() const override {
        return T::getRegisterName();
    }
};



#endif //SOLVERBASE_H
