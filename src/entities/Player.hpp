#pragma once
#include <SDL.h>
#include <string>
#include "input/InputManager.hpp"

namespace entities {

struct PlayerStats {
    int health{5};
    int maxHealth{5};
    int pâte{0};
    int score{0};
    int combo{1};
};

class Player {
public:
    Player();
    void setName(const std::string& name) { name_ = name; }
    void setColor(SDL_Color c) { color_ = c; }
    void setPosition(float x, float y) { x_ = x; y_ = y; }
    void moveBy(float dx, float dy) { x_ += dx; y_ += dy; }
    void setVelocityY(float vy) { vy_ = vy; }
    void setGrounded(bool g) { grounded_ = g; }
    float velocityY() const { return vy_; }
    void applyInput(const input::InputManager& input, float dt);
    void updatePhysics(float dt);
    void render(SDL_Renderer* renderer) const;
    void addCoin();
    void applyDamage(int dmg);
    void grantPowerup(const std::string& powerup);

    SDL_FRect bounds() const { return SDL_FRect{x_, y_, width_, height_}; }
    const PlayerStats& stats() const { return stats_; }

private:
    std::string name_ = "GPU";
    SDL_Color color_{200, 255, 0, 255};
    float x_{64}, y_{64};
    float vx_{0}, vy_{0};
    float width_{48}, height_{48};
    bool grounded_{false};
    bool hasDoubleJump_{true};
    PlayerStats stats_;
};

} // namespace entities
