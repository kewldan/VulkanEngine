#version 450

layout(set = 0, binding = 0) uniform UniformCamera {
    mat4 view;
    mat4 proj;
    vec4 position;
} camera;

layout(push_constant) uniform constants
{
    vec4 color;
    mat4 model;
} PushConstants;

layout(location = 0) in vec3 uPosition;
layout(location = 1) in vec3 uNormal;
layout(location = 2) in vec2 uTexCoord;

layout(location = 0) out VS_OUT {
    vec4 position;
    vec3 color;
    vec3 normal;
    vec3 cameraPosition;
} vs_out;

void main() {
    gl_Position = camera.proj * camera.view * PushConstants.model * vec4(uPosition, 1.0);
    vs_out.position = PushConstants.model * vec4(uPosition, 1.0);
    vs_out.color = PushConstants.color.rgb;
    vs_out.normal = uNormal;
    vs_out.cameraPosition = camera.position.xyz;
}