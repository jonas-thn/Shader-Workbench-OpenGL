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

uniform int index;
uniform vec3 nodeColor;

out vec4 FragColor;

void main()
{
	vec3 lightDir = normalize(vec3(-1.2, 1.5, -1.0));
	float diffuse = max(0.0, dot(lightDir, normalize(Normals)));
	vec3 finalColor = ((diffuse + vec3(0.1)) * nodeColor);

	FragColor = clamp(vec4(finalColor, 1.0), 0.0, 1.0);
}
