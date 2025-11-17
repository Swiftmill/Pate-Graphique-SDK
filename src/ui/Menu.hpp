#pragma once
#include <SDL.h>
#include <string>
#include "discord/DiscordManager.hpp"
#include "levels/Level.hpp"
#include "input/InputManager.hpp"

namespace ui {

class Menu {
public:
    bool handleEvent(const SDL_Event& e, input::InputManager& input, levels::Level& level, discord::DiscordManager& discord);
    void render(SDL_Renderer* renderer, const discord::DiscordManager& discord);

private:
    std::string selectedName_ = "GPU Hero";
};

} // namespace ui
