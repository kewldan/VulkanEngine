#include "Game.h"

void Game::init() {

}

void Game::loop() {

}

void Game::cleanup() {
    window->cleanup();
}

bool Game::shouldClose() {
    return window->shouldClose();
}

void Game::createWindow() {
    window = new Engine::Window("Vulkan game", 800, 600);
}
