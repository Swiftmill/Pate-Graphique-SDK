#include "Game.hpp"
#include <SDL_image.h>
#include <iostream>
#include "entities/Player.hpp"
#include "levels/LevelLoader.hpp"

namespace core {

Game::Game() = default;
Game::~Game() = default;

bool Game::init() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
        return false;
    }

    window_ = SDL_CreateWindow(
        "Pate Graphique: Thermal Run", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1280, 720, SDL_WINDOW_SHOWN);

    if (!window_) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer_) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        return false;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "SDL_image init failed: " << IMG_GetError() << std::endl;
        return false;
    }

    discord_.initialize();
    discord_.setPresence("Cooling down in Thermal Run");

    level_.setRenderer(renderer_);
    levels::LevelLoader loader;
    if (!loader.loadFromFile("assets/levels/demo.json", level_)) {
        std::cerr << "Failed to load level" << std::endl;
    }

    input_.initializeDefaults();
    hud_.setPlayers(level_.players());

    running_ = true;
    lastCounter_ = SDL_GetPerformanceCounter();
    return true;
}

void Game::run() {
    while (running_) {
        Uint64 current = SDL_GetPerformanceCounter();
        float dt = static_cast<float>((current - lastCounter_) / static_cast<double>(SDL_GetPerformanceFrequency()));
        lastCounter_ = current;

        processEvents();
        if (!inMenu_) {
            update(dt);
        }
        render();
    }
}

void Game::shutdown() {
    level_.clear();
    discord_.shutdown();
    if (renderer_) SDL_DestroyRenderer(renderer_);
    if (window_) SDL_DestroyWindow(window_);
    IMG_Quit();
    SDL_Quit();
}

void Game::processEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            running_ = false;
        }
        input_.handleEvent(e);
        if (inMenu_) {
            if (menu_.handleEvent(e, input_, level_, discord_)) {
                inMenu_ = false;
            }
        }
    }
}

void Game::update(float dt) {
    level_.update(dt, input_);
    hud_.update(level_);
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer_, 15, 15, 18, 255);
    SDL_RenderClear(renderer_);
    if (inMenu_) {
        menu_.render(renderer_, discord_);
    } else {
        level_.render(renderer_);
        hud_.render(renderer_);
    }
    SDL_RenderPresent(renderer_);
}

void Game::resetLevel() {
    levels::LevelLoader loader;
    loader.loadFromFile("assets/levels/demo.json", level_);
}

} // namespace core
