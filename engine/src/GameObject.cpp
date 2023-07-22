#include "GameObject.h"
#include "plog/Log.h"

namespace Engine {

    GameObject::GameObject() = default;

    void GameObject::loadMeshes(const char *filename) {
//        assert(filename != nullptr);
//
//        tinyobj::attrib_t attrib;
//        std::vector<tinyobj::shape_t> shapes;
//        std::vector<tinyobj::material_t> materials;
//
//        std::string warn;
//        std::string err;
//
//        tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename, nullptr);
//
//        if (!warn.empty()) {
//            PLOGW << warn;
//        }
//
//        if (!err.empty()) {
//            PLOGE << err;
//            return;
//        }
//
//        for (auto &shape: shapes) {
//            // Loop over faces(polygon)
//            size_t index_offset = 0;
//            for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) {
//
//                //hardcode loading to triangles
//                int fv = 3;
//
//                // Loop over vertices in the face.
//                for (size_t v = 0; v < fv; v++) {
//                    // access to vertex
//                    tinyobj::index_t idx = shape.mesh.indices[index_offset + v];
//
//                    //vertex position
//                    tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
//                    tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
//                    tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];
//                    //vertex normal
//                    tinyobj::real_t nx = attrib.normals[3 * idx.normal_index + 0];
//                    tinyobj::real_t ny = attrib.normals[3 * idx.normal_index + 1];
//                    tinyobj::real_t nz = attrib.normals[3 * idx.normal_index + 2];
//
//                }
//                index_offset += fv;
//            }
//        }
    }
}