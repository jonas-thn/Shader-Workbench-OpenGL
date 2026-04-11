#version 420 core

in vec2 TexCoords;
in vec3 Normals;
in vec3 FragPos;

in vec3 vColor;

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
	vec3 lightDir = normalize(vec3(-1.0, 1.0, 2.0));
	float diffuse = max(0.0, dot(lightDir, normalize(Normals)));
	vec3 finalColor = mix(vColor * diffuse, vColor, 0.7);

	FragColor = vec4(finalColor, 0.0);
}
