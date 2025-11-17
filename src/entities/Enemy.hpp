#pragma once
#include <SDL.h>

namespace entities {

class Enemy {
public:
    Enemy(float x, float y);
    void update(float dt);
    void render(SDL_Renderer* renderer) const;
    SDL_FRect bounds() const { return SDL_FRect{x_, y_, 40, 40}; }
    bool isDefeated() const { return health_ <= 0; }
    void hit(int dmg);

private:
    float x_{0}, y_{0};
    float dir_{1};
    int health_{2};
};

} // namespace entities
