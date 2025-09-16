#pragma once

#include "IModule.h"

#include <memory>
#include <string>
#include <unordered_map>

namespace BlueSapphire {

class ModuleManager {
public:
    static ModuleManager& Get();
    
    void RegisterModule(std::shared_ptr<IModule> module);
    void StartupAll();
    void ShutdownAll();

    std::shared_ptr<IModule> GetModule(const std::string& name);
private:
    ModuleManager() = default;
    std::unordered_map<std::string, std::shared_ptr<IModule>> modules_;
};

} // namespace BlueSapphire