#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "Camera3D.h"

namespace Engine {
    void Camera3D::update(float windowWidth, float windowHeight) {
        assert(windowWidth > 0.f && windowHeight > 0.f);

        projection = glm::perspective(glm::radians(60.f), windowWidth / windowHeight, Z_NEAR, Z_FAR);


        view = glm::rotate(glm::mat4(1.f), rotation.x, glm::vec3(1.f, 0.f, 0.f));
        view = glm::rotate(view, rotation.y, glm::vec3(0.f, 1.f, 0.f));
        view = glm::translate(view, -position);
    }
}