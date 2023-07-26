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
	float3 lightPosition;	//bytes 48-59
	
    int pad0;				//bytes 60-63

	//for direction lights
	float3 lightDirection;	//bytes 64-75

	int lightSourceType;	//bytes 76-79
};

//Make sure the worldNormal, lightVector and viewVector are normalized before passing them in.
float3 GouraudAndPhongShading(Light light, Material mat, float3 worldNormal, float3 lightVector, 
	float3 viewVector, float3 reflectionVector, float attenuation)
{
	//ambient color of the vertex
	float3 ambientColor = light.lAmbient * mat.mAmbient;

	//diffuse color of the vertex
	float kd = dot(lightVector, worldNormal);
	kd = clamp(kd, 0.0f, 1.0f);
	float3 diffuseColor = light.lDiffuse * mat.mDiffuse * kd;

	//specular color of the vertex
	float3 specularColor = float3(0.0f, 0.0f, 0.0f);
	if (kd > 0.0f)
	{
		float ks = dot(reflectionVector, viewVector);
		ks = clamp(ks, 0.0f, 1.0f);
		ks = pow(ks, mat.mShininess);
		specularColor = light.lSpecular * mat.mSpecular * ks;
	}

	//The color of the vertex
	return attenuation * (ambientColor + diffuseColor + specularColor);
}

//Make sure the worldNormal, lightVector and halfwayVector are normalized before passing them in.
float3 BlinnPhongShading(Light light, Material mat, float3 worldNormal, float3 lightVector, float3 halfwayVector,
	float attenuation)
{
	//ambient color of the vertex
	float3 ambientColor = light.lAmbient * mat.mAmbient;

	//diffuse color of the vertex
	float kd = dot(lightVector, worldNormal);
	kd = clamp(kd, 0.0f, 1.0f);
	float3 diffuseColor = light.lDiffuse * mat.mDiffuse * kd;

	//specular color of the vertex
	float3 specularColor = float3(0.0f, 0.0f, 0.0f);
	if (kd > 0.0f)
	{
		float ks = dot(halfwayVector, worldNormal);
		ks = clamp(ks, 0.0f, 1.0f);
		ks = pow(ks, mat.mShininess);
		specularColor = light.lSpecular * mat.mSpecular * ks;
	}

	//The color of the vertex
	return attenuation * (ambientColor + diffuseColor + specularColor);
}

//Make sure the worldNormal is normalized before passing it in.
float3 ComputePointLight(Light light, Material mat, float3 worldNormal, float3 worldPos, float3 cameraPos, uint shadingType)
{
	float3 lightVector = light.lightPosition - worldPos;
	float3 viewVector = cameraPos - worldPos;

	float distance = length(lightVector);
	float attenuation = 1.0f / (1.0f + 0.001f * distance);

	lightVector = normalize(lightVector);
	viewVector = normalize(viewVector);

	float3 col;
	if (shadingType == BLINN_PHONG || shadingType == FLAT_BLINN)
	{
		float3 halfwayVector = lightVector + viewVector;
		halfwayVector = normalize(halfwayVector);
		col = BlinnPhongShading(light, mat, worldNormal, lightVector, halfwayVector, attenuation);
	}
	else //gourad, phong or flat phong shading
	{
		float3 reflectionVector = 2.0f * dot(lightVector, worldNormal) * worldNormal - lightVector;
		reflectionVector = normalize(reflectionVector);
		col = GouraudAndPhongShading(light, mat, worldNormal, lightVector, viewVector, reflectionVector, attenuation);
	}

	return col;
}

//Make sure the worldNormal is normalized before passing it in.
float3 ComputeDirectionalLight(Light light, Material mat, float3 worldNormal, float3 worldPos, float3 cameraPos, uint shadingType)
{
	float3 lightVector = -light.lightDirection;
	float3 viewVector = cameraPos - worldPos;

	lightVector = normalize(lightVector);
	viewVector = normalize(viewVector);

	float3 col;
	if (shadingType == BLINN_PHONG || shadingType == FLAT_BLINN)
	{
		float3 halfwayVector = lightVector + viewVector;
		halfwayVector = normalize(halfwayVector);
		col = BlinnPhongShading(light, mat, worldNormal, lightVector, halfwayVector, 1.0f);
	}
	else //gourad, phong or flat phong shading
	{
		float3 reflectionVector = 2.0f * dot(lightVector, worldNormal) * worldNormal - lightVector;
		reflectionVector = normalize(reflectionVector);
		col = GouraudAndPhongShading(light, mat, worldNormal, lightVector, viewVector, reflectionVector, 1.0f);
	}

	return col;
}

