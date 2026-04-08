#version 330 core

in vec2 TexCoords;
in vec3 Normals;
in vec3 FragPos;

uniform vec3 cameraPos;
uniform int index;
uniform float time;
uniform vec2 resolution;

out vec4 FragColor;


mat3 zrot(float t)
{
    return mat3(cos(t), -sin(t), 0.0,
                sin(t), cos(t), 0.0,
                0.0, 0.0, 1.0);
}

vec2 map(vec3 p)
{
    p.x += sin(p.z);
    p *= zrot(p.z);
    float d = 1000.0;
    vec3 q = fract(p) * 2.0 - 1.0;
    float idx = 0.0;
    for (int i = 0; i < 3; ++i) {
        q = sign(q) * (1.0 - 1.0 / (1.0 + abs(q) * 0.8));
        
        float md = length(q) - 0.5;
        
        float ss = 0.5 + 0.5 * sin(p.z + md * float(i) * 6.0);
        float cyl = length(p.xy) - 0.5 - ss;
        
        md = max(md, -cyl);
        
        if (md < d) {
            d = md;
            idx = float(i);
        }
    } 
    return vec2(d, idx);
}

float Raymach(vec3 rayOrigin, vec3 rayDirection)
{
    float t = 0.0;

    for (int i = 0; i < 64; i++)
    {
        vec3 p = rayDirection * t + rayOrigin;
        float d = map(p).x;

        if(d < 0.001)
        {
            break;
        }

        t += d;

        if(t > 100.0)
        {
            break;
        }
    }	

    return t;
}

void main()
{
    vec2 uv = TexCoords * 2.0 - 1.0;
    uv.x *= resolution.x / resolution.y;
    
    vec3 rayDirection = normalize(vec3(uv, 1.0));
    
    vec3 rayOrigin = vec3(0.0, 0.0, 0.0);
    rayOrigin.z += time;
    rayOrigin.x += -sin(rayOrigin.z) * 0.8;
    
    float dist = Raymach(rayOrigin, rayDirection);
    
    float fog = 1.0 / (1.0 + dist * dist * 0.03);
    fog = pow(fog, 3.0); 
    
    vec3 colorNear = vec3(0.005, 0.00, 0.02);   
    vec3 colorFar = vec3(0.0, 1.0, 0.7);     
    vec3 color = mix(colorFar, colorNear, fog);
    
    FragColor = vec4(color, 1.0);
}