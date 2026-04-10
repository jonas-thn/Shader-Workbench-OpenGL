#version 330 core

in vec2 TexCoords;
in vec3 Normals;
in vec3 FragPos;

uniform vec3 cameraPos;
uniform float time;
uniform vec2 resolution;

out vec4 FragColor;

void main()
{
	FragColor = vec4(
		TexCoords.xy,
		sin(time) * 0.5 + 0.5,
		1.0);
}
