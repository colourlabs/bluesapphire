#pragma once

#include <string>

namespace BlueSapphire {

class IModule {
public:
    virtual ~IModule() = default;

    virtual void Startup() = 0;
    virtual void Shutdown() = 0;
    virtual std::string GetName() const = 0;
};

} // namespace BlueSapphire