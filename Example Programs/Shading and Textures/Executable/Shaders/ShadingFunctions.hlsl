#include "Structures.hlsl"

//Make sure the worldNormal, lightVector and halfwayVector are normalized before passing them in.
float3 BlinnPhongShading(Light light, Material mat, float4 worldNormal, float4 lightVector, float4 halfwayVector,
	float attenuation)
{
	//ambient color of the pixel
	float3 ambientColor = light.color.xyz * mat.ambient.xyz;

	//diffuse color of the pixel
	float kd = dot(lightVector, worldNormal);
	kd = clamp(kd, 0.0f, 1.0f);
	float3 diffuseColor = light.color.xyz * mat.diffuse.xyz * kd;

	//specular color of the pixel
	float3 specularColor = { 0.0f, 0.0f, 0.0f };
	float ks = dot(halfwayVector, worldNormal);
	ks = clamp(ks, 0.0f, 1.0f);
	ks = pow(ks, mat.shininess);
	if (ks > 0.0f)
	{
		specularColor = light.color.xyz * mat.specular.xyz * ks;
		diffuseColor = diffuseColor * (1.0f - ks);
	}

	//The color of the pixel
	return (ambientColor + diffuseColor + specularColor);
}

//Make sure the worldNormal is normalized before passing it in.
float3 ComputePointLightBlinnPhong(Light light, Material mat, float4 worldNormal, float4 worldPos, float4 cameraPos)
{
	float4 lightVector = light.position - worldPos;
	float4 viewVector = cameraPos - worldPos;

	float distance = length(lightVector);
	float attenuation = 1.0f / (1.0f + 0.04f * distance);

	lightVector = normalize(lightVector);
	viewVector = normalize(viewVector);

	float4 halfwayVector = lightVector + viewVector;
	halfwayVector = normalize(halfwayVector);
	
	return BlinnPhongShading(light, mat, worldNormal, lightVector, halfwayVector, attenuation);
}

