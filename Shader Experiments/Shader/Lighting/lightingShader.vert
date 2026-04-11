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
uniform int index;

out vec2 TexCoords;
out vec3 Normals;
out vec3 FragPos;

float hash(float n) {
    return fract(sin(n) * 43758.5453);
}

void main()
{
    vec3 locPos = aPos;

    if(index == 999)
    {
        float t = sin(locPos.y * 20.0 + time * 10.0);
        t += 1.0;
        locPos += aNormals * t * 0.05;
    }

    if(index == 997)
    {
        vec3 pos = normalize(locPos) * length(locPos); 
        float spherify = sin(time) * 0.5 + 0.5; 
        locPos = mix(locPos, normalize(pos), spherify);
    }

	vec4 worldPos = model * vec4(locPos, 1.0);

    if(index == 998)
    {
        float triID = floor(float(gl_VertexID) / 3.0);

        vec3 offset = vec3(
            hash(triID * 12.9),
            hash(triID * 78.2),
            hash(triID * 45.1)
        ) * 2.0 - 1.0;
    
        worldPos.xyz += offset * max(0.0, 0.1 * sin(time) + 0.07);
    }

    FragPos = worldPos.xyz;
    TexCoords = aTexCoords;
    Normals = normalize(transpose(inverse(mat3(model))) * aNormals);

    gl_Position = projection * view * worldPos;
}
