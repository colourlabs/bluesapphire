
#include "App.h"
#include <iostream>

class TestGame : public BlueSapphire::App {
public:
    void OnUpdate(float dt) override {
        std::cout << "Update with dt=" << dt << std::endl;
    }

    bool Initialize(int width, int height, const std::string& title) override {
        std::cout << "Initializing TestGame with width=" << width << ", height=" << height << ", title=" << title << std::endl;
        return true;
    }

    void OnRender() override {
        std::cout << "Render frame" << std::endl;
    }
 };
    

int main() {
    TestGame game;

    if (!game.Initialize(800, 600, "Test Game")) {
        std::cerr << "Failed to initialize game!" << std::endl;
        return -1;
    };

    game.Run();
    game.Shutdown();

    return 0;
}