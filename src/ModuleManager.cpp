#include "ModuleManager.h"
#include "Utilities/Logger.h"

namespace BlueSapphire {

ModuleManager& ModuleManager::Get() {
    static ModuleManager instance;
    return instance;
}

void ModuleManager::RegisterModule(std::shared_ptr<IModule> module) {
    if (!module) {
        throw std::invalid_argument("Module cannot be null");
    }

    const std::string name = module->GetName();
    if (modules_.find(name) != modules_.end()) {
        throw std::runtime_error("Module with name '" + name + "' is already registered");
    }

    modules_[name] = module;
}

void ModuleManager::StartupAll() {
    for (auto& [name, module] : modules_) {
        Utils::Logger::Get().info("Starting module: {}", name);

        module->Startup();
    }
}

void ModuleManager::ShutdownAll() {
    for (auto& [name, module] : modules_) {
        Utils::Logger::Get().info("Shutting down module: {}", name);

        module->Shutdown();
    }
}

std::shared_ptr<IModule> ModuleManager::GetModule(const std::string& name) {
    if (modules_.find(name) != modules_.end())
        return modules_[name];
    
    return nullptr;
}

} // namespace BlueSapphire