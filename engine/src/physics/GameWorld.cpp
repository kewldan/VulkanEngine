#include "physics/GameWorld.h"

namespace Engine {

    GameWorld::GameWorld() = default;

    void GameWorld::init() {
        collisionConfiguration = new btDefaultCollisionConfiguration;
        dispatcher = new btCollisionDispatcher(collisionConfiguration);
        broadphase = new btDbvtBroadphase();
        solver = new btSequentialImpulseConstraintSolver;
        world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
        world->setGravity(btVector3(0, -10, 0));
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