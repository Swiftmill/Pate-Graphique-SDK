#include "HUD.hpp"
#include <string>
#include <SDL.h>
#include <algorithm>

namespace ui {

void HUD::update(const levels::Level& level) {
    (void)level;
}

void HUD::render(SDL_Renderer* renderer) const {
    if (!players_) return;
    int y = 10;
    for (const auto& player : *players_) {
        auto stats = player.stats();
        SDL_Rect bar{10, y, stats.health * 20, 12};
        SDL_SetRenderDrawColor(renderer, 80, 200, 120, 255);
        SDL_RenderFillRect(renderer, &bar);
        y += 20;
    }

    y += 10;
    // Score bars with crude rank color coding
    for (const auto& player : *players_) {
        auto stats = player.stats();
        int rankScore = stats.score;
        SDL_Rect scoreRect{10, y, std::min(400, rankScore / 5 + 40), 10};
        SDL_SetRenderDrawColor(renderer,
            rankScore > 3000 ? 255 : 120,
            rankScore > 1500 ? 200 : 120,
            80,
            220);
        SDL_RenderFillRect(renderer, &scoreRect);
        y += 16;
    }
}

} // namespace ui
