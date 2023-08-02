#include "physics/GameWorld.h"
#include "plog/Log.h"
#include "BulletDynamics/btBulletDynamicsCommon.h"
#include "BulletCollision/btBulletCollisionCommon.h"

namespace Engine {

    GameWorld::GameWorld() = default;

    void GameWorld::init() {
        // Bullet world init

        btDefaultCollisionConfiguration *collisionConfiguration = new btDefaultCollisionConfiguration();

        btCollisionDispatcher *dispatcher = new btCollisionDispatcher(collisionConfiguration);

        btBroadphaseInterface *overlappingPairCache = new btDbvtBroadphase();

        btSequentialImpulseConstraintSolver *solver = new btSequentialImpulseConstraintSolver;

        btDiscreteDynamicsWorld *dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver,
                                                                             collisionConfiguration);

        dynamicsWorld->setGravity(btVector3(0, -10, 0));
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

    void GameWorld::destroy() {

    }
}