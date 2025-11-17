#pragma once
#include <memory>
#include <vector>
#include <string>
#include <SDL.h>
#include "discord/DiscordManager.hpp"
#include "levels/Level.hpp"
#include "input/InputManager.hpp"
#include "ui/HUD.hpp"
#include "ui/Menu.hpp"

namespace core {

class Game {
public:
    Game();
    ~Game();

    bool init();
    void run();
    void shutdown();

private:
    void processEvents();
    void update(float dt);
    void render();
    void resetLevel();

    SDL_Window* window_{};
    SDL_Renderer* renderer_{};
    bool running_{false};

    discord::DiscordManager discord_;
    input::InputManager input_;
    levels::Level level_;
    ui::HUD hud_;
    ui::Menu menu_;
    bool inMenu_{true};
    Uint64 lastCounter_{0};
};

} // namespace core
