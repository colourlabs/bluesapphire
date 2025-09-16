#include "App.h"
#include "AppSettings.h"

#include <iostream>

class TestGame : public BlueSapphire::App {
public:
    void OnUpdate(float dt) override {
        std::cout << "Update with dt=" << dt << std::endl;
    }

    bool Initialize(const BlueSapphire::AppSettings& settings) override {    
        if (!BlueSapphire::App::Initialize(settings))
            return false;

        return true;
    }

    void OnRender() override {
        std::cout << "Render frame" << std::endl;
    }

    void OnFixedUpdate(float dt) override {
        std::cout << "Fixed Update with dt=" << dt << std::endl;
    }
 };
    

int main() {
    TestGame game;
    BlueSapphire::AppSettings settings;

    settings.width = 800;
    settings.height = 600;
    settings.title = "Test Game";
    settings.vsync = true;

    if (game.Initialize(settings)) {
        game.Run();
        game.Shutdown();
    } else {
        return -1;
    };

    return 0;
}