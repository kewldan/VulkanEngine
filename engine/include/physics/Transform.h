#pragma once

#include "glm/glm.hpp"

typedef glm::vec3 Position;
typedef glm::vec3 Rotation;
typedef glm::vec3 Scale;

namespace Engine {
    class Transform {
    public:
        Position position{};
        Rotation rotation{};
        Scale scale = glm::vec3(1.f);

        [[nodiscard]] glm::mat4 getModel() const;
    };
}
