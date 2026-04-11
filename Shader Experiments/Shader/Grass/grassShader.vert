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

out vec3 vColor;

vec3 hash( vec3 p ) 
{
	p = vec3(
        dot(p,vec3(127.1,311.7, 74.7)),
			  dot(p,vec3(269.5,183.3,246.1)),
			  dot(p,vec3(113.5,271.9,124.6)));

	return -1.0 + 2.0*fract(sin(p)*43758.5453123);
}

vec2 hashVec2(float p) {
  vec2 r = vec2(
      dot(vec2(p), vec2(17.43267, 23.8934543)),
      dot(vec2(p), vec2(13.98342, 37.2435232)));
  return fract(sin(r) * 1743.54892229);
}

vec3 hashVec3(vec3 p)
{
	p = vec3(
        dot(p,vec3(127.1,311.7, 74.7)),
			  dot(p,vec3(269.5,183.3,246.1)),
			  dot(p,vec3(113.5,271.9,124.6)));

	return fract(sin(p)*43758.5453123);
}

float noise( in vec3 p )
{
  vec3 i = floor( p );
  vec3 f = fract( p );
	
	vec3 u = f*f*(3.0-2.0*f);

  return mix( mix( mix( dot( hash( i + vec3(0.0,0.0,0.0) ), f - vec3(0.0,0.0,0.0) ), 
                        dot( hash( i + vec3(1.0,0.0,0.0) ), f - vec3(1.0,0.0,0.0) ), u.x),
                   mix( dot( hash( i + vec3(0.0,1.0,0.0) ), f - vec3(0.0,1.0,0.0) ), 
                        dot( hash( i + vec3(1.0,1.0,0.0) ), f - vec3(1.0,1.0,0.0) ), u.x), u.y),
              mix( mix( dot( hash( i + vec3(0.0,0.0,1.0) ), f - vec3(0.0,0.0,1.0) ), 
                        dot( hash( i + vec3(1.0,0.0,1.0) ), f - vec3(1.0,0.0,1.0) ), u.x),
                   mix( dot( hash( i + vec3(0.0,1.0,1.0) ), f - vec3(0.0,1.0,1.0) ), 
                        dot( hash( i + vec3(1.0,1.0,1.0) ), f - vec3(1.0,1.0,1.0) ), u.x), u.y), u.z );
}

mat3 rotateY(float theta) {
  float c = cos(theta);
  float s = sin(theta);
  return mat3(
      vec3(c, 0, s),
      vec3(0, 1, 0),
      vec3(-s, 0, c)
  );
}

const vec3 BASE_COLOR = vec3(0.02, 0.1, 0.01);
const vec3 TIP_COLOR1 = vec3(0.5, 0.7, 0.3);
const vec3 TIP_COLOR2 = vec3(0.88, 0.87, 0.52);

void main()
{
    const float PI = 3.141562;

    //rotation
    float angle = fract(sin(float(gl_InstanceID + 0.1 * index) * 12.9898) * 43758.5453) * 2.0 * PI;
    mat3 rotationMatrix = rotateY(angle);
    vec3 rotatedPos = rotationMatrix * aPos;

    //bend
    float bendAmountA = sin(5 * rotatedPos.y * time) * 0.01; 
    float bendAmountB = sin(time) * 0.5; 
    float bendAmountFinal = -bendAmountA + rotatedPos.y * rotatedPos.y * bendAmountB;
    rotatedPos.x += bendAmountFinal;

    mat3 bendMatrix = mat3(
        1.0, 2.0 * rotatedPos.y * bendAmountFinal, 0.0,  
        0.0, 1.0, 0.0,                              
        0.0, 0.0, 1.0                               
    );

    //offset
    vec2 hashed2D = hashVec2(float(gl_InstanceID + 0.1 * index)) * 2.0 - 1.0;
    vec3 grassOffset = vec3(hashed2D.x, -0.25, hashed2D.y) * 2.0;
	vec3 localPos = rotatedPos + grassOffset;
    vec4 worldPos = model * vec4(localPos, 1.0);

    FragPos = worldPos.xyz;
    TexCoords = aTexCoords;

    vec3 transformedNormal = rotationMatrix * aNormals;
    transformedNormal = bendMatrix * transformedNormal;
    Normals = normalize(mat3(transpose(inverse(model))) * transformedNormal);

    gl_Position = projection * view * worldPos;

    vec3 c1 = mix(BASE_COLOR, TIP_COLOR1, aPos.y / 0.5);
    vec3 c2 = mix(BASE_COLOR, TIP_COLOR2, aPos.y / 0.5);
    float noiseValue = noise(worldPos.xyz);

    vColor = mix(c1, c2, smoothstep(-1.0, 1.0, noiseValue));
}
