#version 420 core

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normals;

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

float hash(float n) { return fract(sin(n) * 43758.5453); }

float noise(vec3 p)
{
    vec3 i = floor(p);
    vec3 f = fract(p);

    vec3 u = f*f*(3.0-2.0*f);

    float n000 = hash(dot(i + vec3(0.0,0.0,0.0), vec3(1.0,57.0,113.0)));
    float n100 = hash(dot(i + vec3(1.0,0.0,0.0), vec3(1.0,57.0,113.0)));
    float n010 = hash(dot(i + vec3(0.0,1.0,0.0), vec3(1.0,57.0,113.0)));
    float n110 = hash(dot(i + vec3(1.0,1.0,0.0), vec3(1.0,57.0,113.0)));
    float n001 = hash(dot(i + vec3(0.0,0.0,1.0), vec3(1.0,57.0,113.0)));
    float n101 = hash(dot(i + vec3(1.0,0.0,1.0), vec3(1.0,57.0,113.0)));
    float n011 = hash(dot(i + vec3(0.0,1.0,1.0), vec3(1.0,57.0,113.0)));
    float n111 = hash(dot(i + vec3(1.0,1.0,1.0), vec3(1.0,57.0,113.0)));

    float nx00 = mix(n000, n100, u.x);
    float nx10 = mix(n010, n110, u.x);
    float nx01 = mix(n001, n101, u.x);
    float nx11 = mix(n011, n111, u.x);

    float nxy0 = mix(nx00, nx10, u.y);
    float nxy1 = mix(nx01, nx11, u.y);

    float nxyz = mix(nxy0, nxy1, u.z);

    return nxyz;
}

void main()
{
	vec3 viewDir = normalize(cameraPos - FragPos);

    vec3 lightDir = normalize(vec3(-1.0, 1.0, -1.0));
	float diffuse = max(0.0, dot(lightDir, normalize(Normals)));
	vec3 baseColor = vec3(0.2) + diffuse;
    vec3 edgeColor = vec3(1.0, 0.8, 0.0);

    float speed = 0.5;
    float sinTime = sin(time * 0.5f) * 0.5 + 0.5;
    float scale = 20.0;

    float noiseValue = noise(vec3(TexCoords.xy * scale, 0.0)); 

    float cutoff = step(sinTime, noiseValue);
    float cutoffEdge = step(sinTime - 0.05, noiseValue);

    vec3 colorMix = mix(baseColor, edgeColor, cutoffEdge);

    vec3 fresnelColor = vec3(0.5, 0.3, 0.0);
	float fresnelValue = 1 - dot(viewDir, normalize(Normals));
	fresnelValue = clamp(pow(fresnelValue, 5.0), 0.0, 1.0);
	vec3 fresnel = fresnelValue * fresnelColor;

    FragColor = vec4(mix(colorMix, vec3(0.0, 0.0, 0.0), cutoff), 1.0);

    if(distance(FragColor.rgb, vec3(0.0)) < 0.01)
    {
        discard;
    }

    FragColor += vec4(fresnel, 0.0);
}
