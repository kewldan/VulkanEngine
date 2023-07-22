#include <glm/ext/matrix_transform.hpp>
#include "Transform.h"

namespace Engine {
    glm::mat4 Transform::getModel() const {
        glm::mat4 model = glm::translate(glm::mat4(1.f), position);
        model = glm::rotate(model, rotation.x, glm::vec3(1.f, 0.f, 0.f));
        model = glm::rotate(model, rotation.y, glm::vec3(0.f, 1.f, 0.f));
        model = glm::rotate(model, rotation.z, glm::vec3(0.f, 0.f, 1.f));
        model = glm::scale(model, scale);
        return model;
    }
}