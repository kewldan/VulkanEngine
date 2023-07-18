#include "Game.h"
#include <Engine.h>

int main() {
    Game game{};

    Engine::Engine::run(game);
    return 0;
}