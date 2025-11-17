#include "Menu.hpp"
#include <iostream>

namespace ui {

bool Menu::handleEvent(const SDL_Event& e, input::InputManager& input, levels::Level& level, discord::DiscordManager& discord) {
    if (e.type == SDL_KEYDOWN && e.key.keysym.scancode == SDL_SCANCODE_RETURN) {
        auto lobby = discord.createOrGetLobby();
        if (level.players().empty()) {
            entities::Player p;
            p.setName(selectedName_);
            level.players().push_back(p);
        }
        std::cout << "Starting game with lobby " << lobby.id << std::endl;
        return true;
    }

    if (e.type == SDL_KEYDOWN && e.key.keysym.scancode == SDL_SCANCODE_F1) {
        input.rebind(input::Action::Jump, SDL_SCANCODE_K);
    }
    return false;
}

void Menu::render(SDL_Renderer* renderer, const discord::DiscordManager& discord) {
    SDL_SetRenderDrawColor(renderer, 20, 20, 25, 255);
    SDL_Rect panel{100, 100, 400, 200};
    SDL_RenderFillRect(renderer, &panel);
    (void)discord;
}

} // namespace ui
