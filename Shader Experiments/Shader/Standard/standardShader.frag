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

out vec4 FragColor;

void main()
{
	vec3 baseColor = vec3(0.2);

	if(index == 800) { baseColor =vec3(0.05, 0.09, 0.05); }

	vec3 lightDir = normalize(vec3(-1.0, 1.0, -1.0));
	float diffuse = max(0.0, dot(lightDir, normalize(Normals)));
	vec3 finalColor = baseColor + diffuse;

	if(index == 700) { finalColor = vec3(1.0, 1.0, 0.9); }

	FragColor = vec4(finalColor, 1.0);
}
