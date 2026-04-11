#version 420 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormals;
layout (location = 2) in vec2 aTexCoords;

layout (std140, binding = 0) uniform SceneUniforms 
{
    mat4 view;
    mat4 projection;
    vec3 cameraPos;
    float padding1; 
    vec2 resolution;
    float time;
    float padding2; 
};

uniform mat4 model;

out vec2 TexCoords;
out vec3 Normals;
out vec3 FragPos;

void main()
{
	vec4 worldPos = model * vec4(aPos, 1.0);
    FragPos = worldPos.xyz;
    TexCoords = aTexCoords;
    Normals = normalize(transpose(inverse(mat3(model))) * aNormals);
    gl_Position = vec4(aPos, 1.0);
}
