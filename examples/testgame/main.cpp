#include "App.h"
#include <iostream>

class TestGame : public BlueSapphire::App {
public:
    void OnUpdate(float dt) override {
        std::cout << "Update with dt=" << dt << std::endl;
    }

    bool Initialize(int width, int height, const std::string& title) override {
        std::cout << "Initializing TestGame with width=" << width << ", height=" << height << ", title=" << title << std::endl;
    
        if (!BlueSapphire::App::Initialize(width, height, title))
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

    if (game.Initialize(800, 600, "Test Game")) {
        game.Run();
        game.Shutdown();
    } else {
        std::cerr << "Failed to initialize the game." << std::endl;
        return -1;
    };

    return 0;
}