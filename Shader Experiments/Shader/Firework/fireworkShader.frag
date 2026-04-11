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

vec2 Hash12(float t)
{
	float x = fract(sin(t * 674.3) * 453.2);
	float y = fract(sin((t+x) * 714.3) * 263.2);

	return vec2(x, y);
}

vec2 Hash12Polar(float t)
{
	float a = fract(sin(t * 674.3) * 453.2) * 6.2832;
	float d = fract(sin((t+a) * 714.3) * 263.2);

	return vec2(sin(a), cos(a)) * d;
}

const int NUM_PARTICLES = 50;
const int NUM_EXPLOSIONS = 7;

float Explosion(vec2 uv, float t)
{
	float sparks = 0;
	for(int i = 0; i < NUM_PARTICLES; i++)
	{
		vec2 dir = Hash12Polar(i + 1) * 0.5;
		float d = length(uv - dir * t);

		float brightness = mix(0.0005 + 0.000002 * i, 0.001 + 0.00002 * i, t);

		brightness *= smoothstep(1.0, 0.6, t);
		brightness *= sin(20 * t + i) * 0.5 + 0.5;
		sparks += brightness/d;
	}

	return sparks;
}

void main()
{
	vec2 sceneUvs = (TexCoords - 0.5) * 2.0;
	vec2 uv = vec2(sceneUvs.r , sceneUvs.g * (resolution.y / resolution.x));

	vec3 baseColor = vec3(0.0);

	for(int i = 0; i < NUM_EXPLOSIONS; i++)
	{
		float t = time + float(i) / NUM_EXPLOSIONS;
		float ft = floor(t);

		vec3 color = sin(vec3(0.34, 0.54, 0.43) * (ft + float(i) * 0.5)) * 0.3 + 0.7;
		vec2 offs = Hash12(float(i) + 1.0 + ft) - 0.4;

		baseColor += Explosion(uv - offs, fract(t)) * color;
	}

	FragColor = vec4(baseColor, 1.0);
}
