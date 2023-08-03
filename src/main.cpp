#include "Game.h"
#include "common/Engine.h"

int main() {
    Game game{};
    Engine::Engine::run(game);
    return 0;
}