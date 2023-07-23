#include "physics/GameWorld.h"
#include "plog/Log.h"

namespace Engine {

    GameWorld::GameWorld() = default;

    void GameWorld::init() {
        foundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback,
                                        gDefaultErrorCallback);

        bool recordMemoryAllocations = true;

        pvd = PxCreatePvd(*foundation);

        physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation,
                                  PxTolerancesScale(), recordMemoryAllocations, pvd);

        PxInitExtensions(*physics, pvd);

        PxSceneDesc desc = PxSceneDesc(PxTolerancesScale());
        desc.gravity = PxVec3(0.f, 9.8f, 0.f);
        desc.cpuDispatcher = PxDefaultCpuDispatcherCreate(4);
        desc.filterShader = PxDefaultSimulationFilterShader;

        scene = physics->createScene(desc);

        /*PxShapeFlags shapeFlags =
                PxShapeFlag::eSCENE_QUERY_SHAPE | PxShapeFlag::eSIMULATION_SHAPE | PxShapeFlag::eVISUALIZATION;
        PxMaterial *materialPtr = physics->createMaterial(0.5f, 0.5f, 0.1f);

        PxRigidDynamic *rigidDynamic = physics->createRigidDynamic(PxTransform(PxVec3(0.f, 2.5f, 0.f)));
        {
            PxShape *shape = physics->createShape(PxBoxGeometry(2.f, 2.f, 2.f), &materialPtr, 1, true, shapeFlags);
            rigidDynamic->attachShape(*shape);
            shape->release(); // this way shape gets automatically released with actor
        }
        rigidDynamic->setMass(5.f);

        scene->addActor(*rigidDynamic);*/
    }

    void GameWorld::cleanup() {
        PxCloseExtensions();
        scene->release();
        physics->release();
        foundation->release();
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