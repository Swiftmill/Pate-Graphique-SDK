#pragma once
#include <SDL.h>
#include <vector>

namespace entities {

struct BossAttack {
    SDL_FRect zone;
    float lifetime{0};
};

class Boss {
public:
    Boss(float x, float y);
    void update(float dt);
    void render(SDL_Renderer* renderer) const;
    SDL_FRect bounds() const { return SDL_FRect{x_, y_, 120, 120}; }
    bool defeated() const { return health_ <= 0; }
    void hit(int dmg);
    const std::vector<BossAttack>& attacks() const { return attacks_; }

private:
    void spawnAttack();

    float x_, y_;
    int health_{20};
    float phaseTimer_{0};
    std::vector<BossAttack> attacks_;
};

} // namespace entities
