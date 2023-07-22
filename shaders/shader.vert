#version 450

layout(binding = 0) uniform UniformBuffer {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(location = 0) in vec3 uPosition;
layout(location = 1) in vec3 uNormal;
layout(location = 2) in vec2 uTexCoord;

layout(location = 0) out vec3 fragColor;

void main() {
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(uPosition, 1.0);
    fragColor = (uNormal + 1) * 0.5;
}