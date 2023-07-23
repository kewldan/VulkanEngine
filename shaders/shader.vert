#version 450

layout(set = 0, binding = 0) uniform UniformCamera {
    mat4 view;
    mat4 proj;
} camera;

layout(push_constant) uniform constants
{
    mat4 model;
} PushConstants;

layout(location = 0) in vec3 uPosition;
layout(location = 1) in vec3 uNormal;
layout(location = 2) in vec2 uTexCoord;

layout(location = 0) out vec3 fragColor;

void main() {
    gl_Position = camera.proj * camera.view * PushConstants.model * vec4(uPosition, 1.0);
    fragColor = (uNormal + 1) * 0.5;
}