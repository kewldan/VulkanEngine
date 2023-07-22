#pragma once

#include "Camera.h"

namespace Engine {
    class Camera3D : public Camera {
    public:
        void update(float windowWidth, float windowHeight) override;
    };
}
