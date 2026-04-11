#version 420 core

in vec2 TexCoords;
in vec3 Normals;
in vec3 FragPos;

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

out vec4 FragColor;

void main()
{
	FragColor = vec4(
		TexCoords.xy,
		sin(time) * 0.5 + 0.5,
		1.0);
}