#version 330 core

in vec2 TexCoords;
in vec3 Normals;
in vec3 FragPos;

uniform vec3 cameraPos;
uniform int index;
uniform float time;
uniform vec2 resolution;

out vec4 FragColor;

float Math_Random(vec2 p) 
{
  p  = 50.0*fract( p*0.3183099 + vec2(0.71,0.113));
  return -1.0+2.0*fract( p.x*p.y*(p.x+p.y) );
}

float noise(vec2 coords) {
  vec2 texSize = vec2(1.0);
  vec2 pc = coords * texSize;
  vec2 base = floor(pc);

  float s1 = Math_Random((base + vec2(0.0, 0.0)) / texSize);
  float s2 = Math_Random((base + vec2(1.0, 0.0)) / texSize);
  float s3 = Math_Random((base + vec2(0.0, 1.0)) / texSize);
  float s4 = Math_Random((base + vec2(1.0, 1.0)) / texSize);

  vec2 f = smoothstep(0.0, 1.0, fract(pc));

  float px1 = mix(s1, s2, f.x);
  float px2 = mix(s3, s4, f.x);
  float result = mix(px1, px2, f.y);
  return result;
}

float noiseFBM(vec2 p, int octaves, float persistence, float lacunarity) {
  float amplitude = 0.5;
  float total = 0.0;

  for (int i = 0; i < octaves; ++i) {
    float noiseValue = noise(p);
    total += noiseValue * amplitude;
    amplitude *= persistence;
    p = p * lacunarity;
  }

  return total;
}

struct MaterialData {
	vec3 color;
	float dist;
};

MaterialData opU(MaterialData a, MaterialData b)
{
	if(a.dist < b.dist)
	{
		return a;
	}
	return b;
}

MaterialData map(vec3 pos)
{
	float curNoiseSample = noiseFBM(pos.xz / 2.0, 1, 0.5, 2.0);
	curNoiseSample = abs(curNoiseSample);
	curNoiseSample *= 1.5;
	curNoiseSample += 0.1 * noiseFBM(pos.xz * 4.0, 6, 0.5, 2.0);

	float WATER_LEVEL = 0.45;

	vec3 landColor = vec3(0.5, 0.44, 0.4);
	landColor = mix(landColor, landColor * 0.25, smoothstep(WATER_LEVEL - 0.1, WATER_LEVEL, curNoiseSample));

	MaterialData result = MaterialData(landColor, pos.y + curNoiseSample);

	vec3 shallowColor = vec3(0.25, 0.25, 0.75);
	vec3 deepColor = vec3(0.025, 0.025, 0.15);
	vec3 waterColor = mix(shallowColor, deepColor, smoothstep(WATER_LEVEL, WATER_LEVEL + 0.1, curNoiseSample));
	waterColor = mix(waterColor, vec3(1.0), smoothstep(WATER_LEVEL + 0.0125, WATER_LEVEL, curNoiseSample));

	MaterialData waterMaterial = MaterialData(waterColor, pos.y + WATER_LEVEL);

	result = opU(result, waterMaterial);

	return result;
}

const int NUM_STEPS = 256;
const float MAX_DIST = 1000.0;
const float MIN_DIST = 0.00001;

MaterialData RayCast(vec3 cameraOrigin, vec3 cameraDir, int numSteps, float starDist, float maxDist)
{
	MaterialData material = MaterialData(vec3(0.0), starDist);
	MaterialData defaultMaterial = MaterialData(vec3(0.0), -1.0);

	for(int i = 0; i < NUM_STEPS; i++)
	{
		vec3 pos = cameraOrigin + material.dist * cameraDir;

		MaterialData result = map(pos);

		if(abs(result.dist) < MIN_DIST * material.dist)
		{
			break;
		}

		material.dist += result.dist;
		material.color = result.color;

		if(material.dist > MAX_DIST)
		{
			return defaultMaterial;
		}
	}

	return material;
}

float CalculateShadow(vec3 pos, vec3 lightDir)
{
	MaterialData result = RayCast(pos, lightDir, 64, 0.01, 10.0);

	if(result.dist >= 0.0)
	{
		return 0.0;
	}

	return 1.0;
}


vec3 CalculateNormal(vec3 pos)
{
	const float EPS = 0.0001;
	vec3 n = vec3(
		map(pos + vec3(EPS, 0.0, 0.0)).dist - map(pos - vec3(EPS, 0.0, 0.0)).dist, 
		map(pos + vec3(0.0, EPS, 0.0)).dist - map(pos - vec3(0.0, EPS, 0.0)).dist, 
		map(pos + vec3(0.0, 0.0, EPS)).dist - map(pos - vec3(0.0, 0.0, EPS)).dist
	);

	return normalize(n);
}

vec3 CalculateLighting(vec3 pos, vec3 normal, vec3 lightColor, vec3 lightDir)
{
	float dp = clamp((dot(normal, lightDir)), 0.0, 1.0);

	return lightColor * dp;
}


vec3 Raymarch(vec3 cameraOrigin, vec3 cameraDir)
{
	MaterialData material = RayCast(cameraOrigin,cameraDir, NUM_STEPS, 1.0, MAX_DIST);

	vec3 lightDir = normalize(vec3(-0.5, 0.2, -0.6));
	vec3 lightColor = vec3(1.0);

	float skyT = exp(clamp((cameraDir.y * -40.0), 0.0, 1.0));
	float sunFactor = pow(clamp(dot(lightDir, cameraDir), 0.0, 1.0), 8.0);
	vec3 skyColor = mix(vec3(0.025, 0.065, 0.5), vec3(0.4, 0.5, 1.0), skyT); 
	skyColor = mix(skyColor, vec3(1.0, 0.9, 0.65), sunFactor);
	vec3 fogColor = mix(skyColor, vec3(1.0, 0.9, 0.65), sunFactor);

	if(material.dist < 0.0)
	{
		return skyColor;
	}

	vec3 pos = cameraOrigin + material.dist * cameraDir;
	
	vec3 normal = CalculateNormal(pos);
	float shadows = CalculateShadow(pos, lightDir);
	vec3 lighting = CalculateLighting(pos, normal, lightColor, lightDir);
	lighting *= shadows;
	vec3 color = material.color * lighting;

	float fogDist = distance(cameraOrigin, pos);
	float inscatter = 1.0 - exp(-fogDist * fogDist * mix(0.0005, 0.001, sunFactor));
	float extinction = exp(-fogDist * fogDist * 0.01);

	color = color * extinction  + fogColor * inscatter;



	return color;
}

mat3 MakeCameraMatrix(vec3 cameraOrigin, vec3 cameraLookAt, vec3 cameraUp)
{
	vec3 z = normalize(cameraLookAt - cameraOrigin);
	vec3 x = normalize(cross(z, cameraUp));
	vec3 y = cross(x, z);
	return mat3(x, y, z);
}

void main()
{
	vec2 pixelCoords = vec2((TexCoords - 0.5).r * resolution.x, (TexCoords - 0.5).g * resolution.y);

	vec2 sceneUvs = (TexCoords - 0.5) * 2.0;
	vec2 correctedUvs = vec2(sceneUvs.r , sceneUvs.g * (resolution.y / resolution.x));

	float t = time * -0.5;
	vec3 rayDir = normalize(vec3(correctedUvs, 1.0));
	vec3 rayOrigin = vec3(3.0, 0.7, -3.0) * vec3(cos(t), 1.0, sin(t));
	vec3 rayLookAt = vec3(0.0);
	mat3 camera = MakeCameraMatrix(rayOrigin, rayLookAt, vec3(0.0, 1.0, 0.0));

	vec3 baseColor = Raymarch(rayOrigin, camera* rayDir);

	FragColor = vec4(baseColor, 1.0);
    FragColor = pow(FragColor, vec4(1/1.2));
}
