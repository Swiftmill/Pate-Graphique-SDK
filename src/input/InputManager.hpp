#pragma once
#include <SDL.h>
#include <map>
#include <string>
#include <vector>

namespace input {

enum class Action {
    MoveLeft,
    MoveRight,
    Jump,
    Attack,
    Pause,
    Count
};

struct Binding {
    SDL_Scancode key;
};

class InputManager {
public:
    void initializeDefaults();
    void handleEvent(const SDL_Event& e);
    bool isDown(Action action) const;
    void rebind(Action action, SDL_Scancode key);

    const std::map<Action, Binding>& bindings() const { return bindings_; }

private:
    std::map<Action, Binding> bindings_;
    std::map<SDL_Scancode, bool> keyState_;
};

} // namespace input
