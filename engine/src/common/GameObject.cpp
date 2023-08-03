#include "common/GameObject.h"
#include "OBJ_Loader.h"

namespace Engine {
    void GameObject::destroy() {
        for (int i = 0; i < meshCount; i++) {
            meshes[i].destroy();
        }
    }

    void GameObject::load() {
        objl::Loader Loader;
        bool loadout = Loader.LoadFile(filename);

        meshCount = Loader.LoadedMeshes.size();
        meshes = new Mesh[meshCount];

        if (loadout) {
            int i = 0;
            for (const auto &curMesh: Loader.LoadedMeshes) {
                auto *vertices = new Vertex[curMesh.Vertices.size()];
                int j = 0;
                for (const auto &Vert: curMesh.Vertices) {
                    vertices[j].pos = glm::vec3(Vert.Position.X, Vert.Position.Y, Vert.Position.Z);
                    vertices[j].normal = glm::vec3(Vert.Normal.X, Vert.Normal.Y, Vert.Normal.Z);
                    vertices[j].uv = glm::vec2(Vert.TextureCoordinate.X, Vert.TextureCoordinate.Y);
                    j++;
                }
                auto *indices = new uint16_t[curMesh.Indices.size()];
                for (int k = 0; k < curMesh.Indices.size(); k++) {
                    indices[k] = curMesh.Indices[k];
                }
                meshes[i] = Mesh(vertices, indices, curMesh.Indices.size(), curMesh.Vertices.size());
                meshes[i].upload();
                i++;
            }
        }
    }

    GameObject::GameObject(const char *filename, float mass) : filename(
            filename) {
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, new btDefaultMotionState(btTransform::getIdentity()),
                                                        new btBoxShape(btVector3(1.f, 1.f, 1.f)));
        rb = new btRigidBody(rbInfo);
    }

    void GameObject::getModel(glm::mat4 *mvp) const {
        rb->getWorldTransform().getOpenGLMatrix(reinterpret_cast<btScalar *>(mvp));
    }

    GameObject::GameObject() = default;
}