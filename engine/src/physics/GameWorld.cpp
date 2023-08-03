#include "physics/GameWorld.h"
#include "plog/Log.h"
#include "BulletCollision/btBulletCollisionCommon.h"
#include "graphics/RenderPipeline.h"
#include "io/AssetLoader.h"
#include "misc/Timer.h"
#include "imgui.h"

namespace Engine {

    GameWorld::GameWorld() = default;

    void GameWorld::init() {

        auto *collisionConfiguration = new btDefaultCollisionConfiguration();

        auto *dispatcher = new btCollisionDispatcher(collisionConfiguration);

        btBroadphaseInterface *overlappingPairCache = new btDbvtBroadphase();

        auto *solver = new btSequentialImpulseConstraintSolver;

        dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver,
                                                    collisionConfiguration);
        dynamicsWorld->setGravity(btVector3(0, -10, 0));
    }

    void GameWorld::render(const RenderPipeline &pipeline) {
        for (const GameObject *gameObject: gameObjects) {
            pipeline.renderFunction(*gameObject);
        }
    }

    GameObject *GameWorld::instantiate() {
        auto *gameObject = new GameObject();

        gameObjects.push_back(gameObject);

        return gameObject;
    }

    GameObject *GameWorld::instantiate(const char *filename, float mass, btCollisionShape *shape, btVector3 position) {
        assert(filename != nullptr);

        auto *gameObject = new GameObject(filename, mass, shape, position);

        gameObjects.push_back(gameObject);

        dynamicsWorld->addRigidBody(gameObject->rb);
        return gameObject;
    }

    void GameWorld::destroy() {
        for (GameObject *gameObject: gameObjects) {
            gameObject->destroy();
        }

        delete dynamicsWorld;
    }

    void GameWorld::load() {
        for (GameObject *gameObject: gameObjects) {
            AssetLoader::loadAsset(*gameObject);
        }
    }

    void GameWorld::update() {
        dynamicsWorld->stepSimulation(ImGui::GetIO().DeltaTime);
    }
}