#include "Player.hpp"
#include <algorithm>

namespace entities {

Player::Player() = default;

void Player::applyInput(const input::InputManager& input, float dt) {
    const float speed = 260.0f;
    if (input.isDown(input::Action::MoveLeft)) {
        vx_ = -speed;
    } else if (input.isDown(input::Action::MoveRight)) {
        vx_ = speed;
    } else {
        vx_ *= 0.8f;
    }

    if (input.isDown(input::Action::Jump)) {
        if (grounded_) {
            vy_ = -480.0f;
            grounded_ = false;
            hasDoubleJump_ = true;
        } else if (hasDoubleJump_) {
            vy_ = -420.0f;
            hasDoubleJump_ = false;
        }
    }

    vy_ += 980.0f * dt; // gravity
    x_ += vx_ * dt;
    y_ += vy_ * dt;

    if (y_ > 640) { // floor
        y_ = 640;
        vy_ = 0;
        grounded_ = true;
        hasDoubleJump_ = true;
        stats_.combo = 1;
    }
}

void Player::updatePhysics(float dt) {
    // Placeholder for collision resolution with tiles, handled by Level
    (void)dt;
}

void Player::render(SDL_Renderer* renderer) const {
    SDL_FRect rect{x_, y_, width_, height_};
    SDL_SetRenderDrawColor(renderer, color_.r, color_.g, color_.b, color_.a);
    SDL_RenderFillRectF(renderer, &rect);
}

void Player::addCoin() {
    stats_.pâte += 1;
    stats_.score += 100 * stats_.combo;
    stats_.combo = std::min(stats_.combo + 1, 8);
}

void Player::applyDamage(int dmg) {
    stats_.health = std::max(0, stats_.health - dmg);
    stats_.combo = 1;
}

void Player::grantPowerup(const std::string& powerup) {
    if (powerup == "shield") {
        stats_.health = std::min(stats_.maxHealth, stats_.health + 1);
    } else if (powerup == "speed") {
        vx_ *= 1.2f;
    }
}

} // namespace entities
