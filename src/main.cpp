#include "core/Game.hpp"
#include <SDL.h>
#include <iostream>

int main(int argc, char** argv) {
    (void)argc; (void)argv;
    try {
        core::Game game;
        if (!game.init()) {
            std::cerr << "Failed to initialize game" << std::endl;
            return 1;
        }
        game.run();
        game.shutdown();
    } catch (const std::exception& ex) {
        std::cerr << "Fatal error: " << ex.what() << std::endl;
        return 1;
    }
    return 0;
}
