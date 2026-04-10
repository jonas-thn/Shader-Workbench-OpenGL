#version 330 core

in vec2 TexCoords;
in vec3 Normals;
in vec3 FragPos;

uniform vec3 cameraPos;
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
