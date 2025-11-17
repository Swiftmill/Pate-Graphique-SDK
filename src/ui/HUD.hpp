#pragma once
#include <SDL.h>
#include <vector>
#include "entities/Player.hpp"
#include "levels/Level.hpp"

namespace ui {

class HUD {
public:
    void setPlayers(const std::vector<entities::Player>& players) { players_ = &players; }
    void update(const levels::Level& level);
    void render(SDL_Renderer* renderer) const;

private:
    const std::vector<entities::Player>* players_{nullptr};
};

} // namespace ui
