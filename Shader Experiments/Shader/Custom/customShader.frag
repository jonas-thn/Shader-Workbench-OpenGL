#version 330 core

in vec2 TexCoords;
in vec3 Normals;
in vec3 FragPos;

uniform vec3 cameraPos;
uniform int index;
uniform float time;
uniform vec2 resolution;

out vec4 FragColor;

void main()
{
	FragColor = vec4(TexCoords.xy, 0.0, 1.0);
}
