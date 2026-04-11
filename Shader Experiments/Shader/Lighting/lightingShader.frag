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

void main()
{
	//base
	vec3 baseColor = vec3(1.0);
	if(index == 0)
	{
		baseColor = vec3(1.0, 0.0, 0.0);
	}
	else if (index == 1)
	{
		baseColor = vec3(.0, 1.0, 0.0);
	}
	else if (index == 2)
	{
		baseColor = vec3(0.0, 0.5, 1.0);
	}
	else if (index == 3)
	{
		baseColor = vec3(1.0, 1.0, 0.0);
	}
	else if (index == 4)
	{
		baseColor = vec3(1.0, 0.0, 1.0);
	}

	vec3 normal = normalize(Normals);
	vec3 viewDir = normalize(cameraPos - FragPos);

	//ambient
	vec3 ambient = vec3(0.09, 0.09, 0.11);

	//hemi
	vec3 skyColor = vec3(0.0, 0.02, 0.1);
	vec3 groundColor = vec3(0.1, 0.04, 0.0);
	float hemiMix = (normal.y + 1.0) * 0.5;
	vec3 hemi = mix(groundColor, skyColor, hemiMix);

	//diffuse
	vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));
	vec3 lightColor = vec3(0.9, 0.85, 0.75);
	float diffuseAmount = max(0.0, dot(lightDir, normal));
	vec3 diffuse = diffuseAmount * lightColor;

	//specular
	vec3 reflection = normalize(reflect(-lightDir, normal));
	float specularValue = pow(max(0.0, dot(viewDir, reflection)), 64.0);
	vec3 specular = vec3(specularValue);

	//fresnel
	vec3 fresnelColor = vec3(0.9, 0.8, 0.5);
	float fresnelValue = 1 - dot(viewDir, normal);
	fresnelValue = clamp(pow(fresnelValue, 5.0), 0.0, 1.0);
	vec3 fresnel = fresnelValue * fresnelColor;

	//final
	float saturation = 0.5;
	vec3 finalColor = (baseColor * ambient) + (hemi * 0.5) + (diffuse * saturation + diffuse * baseColor * saturation) + (specular * 0.5) + (fresnel * 0.2);
	finalColor = clamp(finalColor, 0.0, 1.0);

	 if(index == 999)
	 {
		 fresnelColor = vec3(0.0, 0.3, 0.5);
		fresnelValue = 1 - dot(viewDir, normal);
		fresnelValue = clamp(pow(fresnelValue, 5.0), 0.0, 1.0);
		fresnel = fresnelValue * fresnelColor;

		finalColor += fresnel;
	 }
	 else if(index == 998)
	 {
		fresnelColor = vec3(0.6, 0.0, 0.0);
		fresnelValue = 1 - dot(viewDir, normal);
		fresnelValue = clamp(pow(fresnelValue, 5.0), 0.0, 1.0);
		fresnel = fresnelValue * fresnelColor;

		finalColor += fresnel;
	 }
	 else if(index == 997)
	 {
		fresnelColor = vec3(0.0, 0.6, 0.0);
		fresnelValue = 1 - dot(viewDir, normal);
		fresnelValue = clamp(pow(fresnelValue, 5.0), 0.0, 1.0);
		fresnel = fresnelValue * fresnelColor;

		finalColor += fresnel;
	 }

	FragColor = vec4(finalColor, 1.0);
}
