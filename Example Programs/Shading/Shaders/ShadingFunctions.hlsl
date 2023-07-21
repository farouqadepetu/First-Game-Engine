#define GOURAUD 0
#define PHONG 1
#define BLINN_PHONG 2
#define FLAT_PHONG 3
#define FLAT_BLINN 4

#define MAX_NUM_LIGHTS 4
#define POINT_LIGHT 0
#define DIRECTIONAL_LIGHT 1

//Material properties
struct Material
{
	float4 mAmbient;	//bytes 0-15
	float4 mDiffuse;	//bytes 16-31
	float4 mSpecular;	//bytes 32-47
	float mShininess;	//bytes 48-51
};

//Light properties
struct Light
{
	float4 lAmbient;		//bytes 0-15
	float4 lDiffuse;		//bytes 16-31
	float4 lSpecular;		//bytes 32-47

	//for point lights	
	float4 lightPosition;	//bytes 48-63

	//for direction lights
	float4 lightDirection;	//bytes 64-79

	int lightSourceType;	//bytes 80-83

	float3 pad;				//bytes 84-95
};

//Make sure the worldNormal, lightVector and viewVector are normalized before passing them in.
float3 GouraudAndPhongShading(Light light, Material mat, float4 worldNormal, float4 lightVector, 
	float4 viewVector, float4 reflectionVector, float attenuation)
{
	//ambient color of the vertex
	float3 ambientColor = light.lAmbient.xyz * mat.mAmbient.xyz;

	//diffuse color of the vertex
	float kd = dot(lightVector, worldNormal);
	kd = clamp(kd, 0.0f, 1.0f);
	float3 diffuseColor = light.lDiffuse.xyz * mat.mDiffuse.xyz * kd;

	//specular color of the vertex
	float3 specularColor = float3(0.0f, 0.0f, 0.0f);
	if (kd > 0.0f)
	{
		float ks = dot(reflectionVector, viewVector);
		ks = clamp(ks, 0.0f, 1.0f);
		ks = pow(ks, mat.mShininess);
		specularColor = light.lSpecular.xyz * mat.mSpecular.xyz * ks;
	}

	//The color of the vertex
	return attenuation * (ambientColor + diffuseColor + specularColor);
}

//Make sure the worldNormal, lightVector and halfwayVector are normalized before passing them in.
float3 BlinnPhongShading(Light light, Material mat, float4 worldNormal, float4 lightVector, float4 halfwayVector,
	float attenuation)
{
	//ambient color of the vertex
	float3 ambientColor = light.lAmbient.xyz * mat.mAmbient.xyz;

	//diffuse color of the vertex
	float kd = dot(lightVector, worldNormal);
	kd = clamp(kd, 0.0f, 1.0f);
	float3 diffuseColor = light.lDiffuse.xyz * mat.mDiffuse.xyz * kd;

	//specular color of the vertex
	float3 specularColor = float3(0.0f, 0.0f, 0.0f);
	if (kd > 0.0f)
	{
		float ks = dot(halfwayVector, worldNormal);
		ks = clamp(ks, 0.0f, 1.0f);
		ks = pow(ks, mat.mShininess);
		specularColor = light.lSpecular.xyz * mat.mSpecular.xyz * ks;
	}

	//The color of the vertex
	return attenuation * (ambientColor + diffuseColor + specularColor);
}

//Make sure the worldNormal is normalized before passing it in.
float3 ComputePointLight(Light light, Material mat, float4 worldNormal, float4 worldPos, float4 cameraPos, uint shadingType)
{
	float4 lightVector = light.lightPosition - worldPos;
	float4 viewVector = cameraPos - worldPos;

	float distance = length(lightVector);
	float attenuation = 1.0f / (1.0f + 0.001f * distance);

	lightVector = normalize(lightVector);
	viewVector = normalize(viewVector);

	float3 col;
	if (shadingType == BLINN_PHONG || shadingType == FLAT_BLINN)
	{
		float4 halfwayVector = lightVector + viewVector;
		halfwayVector = normalize(halfwayVector);
		col = BlinnPhongShading(light, mat, worldNormal, lightVector, halfwayVector, attenuation);
	}
	else //gourad, phong or flat phong shading
	{
		float4 reflectionVector = 2.0f * dot(lightVector, worldNormal) * worldNormal - lightVector;
		reflectionVector = normalize(reflectionVector);
		col = GouraudAndPhongShading(light, mat, worldNormal, lightVector, viewVector, reflectionVector, attenuation);
	}

	return col;
}

//Make sure the worldNormal is normalized before passing it in.
float3 ComputeDirectionalLight(Light light, Material mat, float4 worldNormal, float4 worldPos, float4 cameraPos, uint shadingType)
{
	float4 lightVector = -light.lightDirection;
	float4 viewVector = cameraPos - worldPos;

	lightVector = normalize(lightVector);
	viewVector = normalize(viewVector);

	float3 col;
	if (shadingType == BLINN_PHONG || shadingType == FLAT_BLINN)
	{
		float4 halfwayVector = lightVector + viewVector;
		halfwayVector = normalize(halfwayVector);
		col = BlinnPhongShading(light, mat, worldNormal, lightVector, halfwayVector, 1.0f);
	}
	else //gourad, phong or flat phong shading
	{
		float4 reflectionVector = 2.0f * dot(lightVector, worldNormal) * worldNormal - lightVector;
		reflectionVector = normalize(reflectionVector);
		col = GouraudAndPhongShading(light, mat, worldNormal, lightVector, viewVector, reflectionVector, 1.0f);
	}

	return col;
}

