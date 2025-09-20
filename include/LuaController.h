#pragma once

#include "Component.h"

#include <sol2/sol.hpp>

namespace BlueSapphire {

class LuaController : public Component {
public:
    sol::function updateFunc;

    void Update(float dt, GameObject* owner) override {
        if (updateFunc.valid())
            updateFunc(owner, dt);
    }
};

};