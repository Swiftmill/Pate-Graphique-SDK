#include "Enemy.hpp"

namespace entities {

Enemy::Enemy(float x, float y) : x_(x), y_(y) {}

void Enemy::update(float dt) {
    const float speed = 60.0f;
    x_ += dir_ * speed * dt;
    if (x_ < 64 || x_ > 1180) {
        dir_ *= -1.0f;
    }
}

void Enemy::render(SDL_Renderer* renderer) const {
    SDL_FRect rect{x_, y_, 40, 40};
    SDL_SetRenderDrawColor(renderer, 240, 70, 70, 255);
    SDL_RenderFillRectF(renderer, &rect);
}

void Enemy::hit(int dmg) {
    health_ -= dmg;
}

} // namespace entities
