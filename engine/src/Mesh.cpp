#include "Mesh.h"

namespace Engine {
    Mesh::Mesh(Vertex *vertices, uint16_t *indices, int indexCount, int vertexCount) : vertices(vertices),
                                                                                       indices(indices),
                                                                                       indexCount(indexCount),
                                                                                       vertexCount(vertexCount) {}

    Mesh::Mesh() = default;
}