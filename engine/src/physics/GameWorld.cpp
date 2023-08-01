#include "physics/GameWorld.h"
#include "plog/Log.h"

namespace Engine {

    GameWorld::GameWorld() = default;

    void GameWorld::init() {

    }

    void GameWorld::cleanup() {
        
    }

    void GameWorld::render() {
        for (const GameObject *gameObject: gameObjects) {

        }
    }

    GameObject *GameWorld::instantiate() {
        auto *gameObject = new GameObject();

        gameObjects.push_back(gameObject);

        return gameObject;
    }
}