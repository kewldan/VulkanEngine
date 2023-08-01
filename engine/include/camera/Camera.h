#pragma once

#include "glm/glm.hpp"
#include "physics/Transform.h"

namespace Engine {
    class Camera : public Transform {
    protected:
        glm::mat4 view{}, projection{};
        float Z_NEAR, Z_FAR;
    public:
        explicit Camera(float zNear = 0.01f, float zFar = 300.f);

        virtual void update(float windowWidth, float windowHeight) = 0;

        [[nodiscard]] const virtual glm::mat4 &getView() const;

        [[nodiscard]] const virtual glm::mat4 &getProjection() const;
    };
}
