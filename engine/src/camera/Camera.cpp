#include "camera/Camera.h"

namespace Engine {
    const glm::mat4 &Camera::getView() const {
        return view;
    }

    const glm::mat4 &Camera::getProjection() const {
        return projection;
    }

    Camera::Camera(float zNear, float zFar) : Z_NEAR(zNear), Z_FAR(zFar) {}
}