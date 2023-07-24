#version 450

layout(location = 0) in VS_OUT {
    vec4 position;
    vec3 color;
    vec3 normal;
    vec3 cameraPosition;
} fs_in;

layout(location = 0) out vec4 FragColor;

void main() {
    vec3 lightPos = vec3(5, 5, 5);

    vec3 ambient = 0.05 * fs_in.color;
    // diffuse
    vec3 lightDir = normalize(lightPos - fs_in.position.xyz);
    vec3 normal = normalize(fs_in.normal);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * fs_in.color;
    // specular
    vec3 viewDir = normalize(fs_in.cameraPosition - fs_in.position.xyz);
    vec3 reflectDir = reflect(-lightDir, normal);

    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    vec3 specular = vec3(0.3) * spec;// assuming bright white light color
    FragColor = vec4(ambient + diffuse + specular, 1.0);
}