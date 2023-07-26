#include "Structures.hlsl"

//Make sure the worldNormal, lightVector and halfwayVector are normalized before passing them in.
float3 BlinnPhongShading(Light light, Material mat, float3 worldNormal, float3 lightVector, float3 halfwayVector,
	float attenuation)
{
	//ambient color of the pixel
	float3 ambientColor = light.color * mat.ambient;

	//diffuse color of the pixel
	float kd = dot(lightVector, worldNormal);
	kd = clamp(kd, 0.0f, 1.0f);
	float3 diffuseColor = light.color * mat.diffuse * kd;

	//specular color of the pixel
	float3 specularColor = { 0.0f, 0.0f, 0.0f };
	float ks = dot(halfwayVector, worldNormal);
	ks = clamp(ks, 0.0f, 1.0f);
	ks = pow(ks, mat.shininess);
	if (ks > 0.0f)
	{
		specularColor = light.color * mat.specular * ks;
		diffuseColor = diffuseColor * (1.0f - ks);
	}

	//The color of the pixel
	return (ambientColor + diffuseColor + specularColor);
}

//Make sure the worldNormal is normalized before passing it in.
float3 ComputePointLightBlinnPhong(Light light, Material mat, float3 worldNormal, float3 worldPos, float3 cameraPos)
{
	float3 lightVector = light.position - worldPos;
	float3 viewVector = cameraPos - worldPos;

	float distance = length(lightVector);
	float attenuation = 1.0f / (1.0f + 0.04f * distance);

	lightVector = normalize(lightVector);
	viewVector = normalize(viewVector);

	float3 halfwayVector = lightVector + viewVector;
	halfwayVector = normalize(halfwayVector);
	
	return BlinnPhongShading(light, mat, worldNormal, lightVector, halfwayVector, attenuation);
}

