//output struct
struct vertexOutput
{
	float4 outputPosition : SV_POSITION;
	float4 worldPosition: POSITION;
	float4 worldNormal : NORMAL;
};

//pass constant buffer
cbuffer PassConstantBuffer : register(b1)
{
	float4x4 viewMatrix;		//bytes 0-63
	float4x4 projectionMatrix;  //bytes 64-127
	float3 cameraPosition;		//bytes 128-139

	float4x4 pad2;				//bytes 140-203
	float4 pad3;				//bytes 204-219
	float4 pad4;				//bytes 220-235
	float4 pad5;				//bytes 236-251
	float pad6;					//bytes 252-255
};

//Material properties
cbuffer Material : register(b2)
{
	float4 mAmbient;	//bytes 0-15
	float4 mDiffuse;	//bytes 16-31
	float4 mSpecular;	//bytes 32-47
	float mShininess;	//bytes 48-51

	float4x4 padding0;	//bytes 52-115
	float4x4 padding1;	//bytes 116-179
	float4x4 padding2;	//bytes 180-243
	float3 padding3;	//bytes 244-255
};

//Light properties
cbuffer Light : register (b3)
{
	float4 lAmbient;		//bytes 0-15
	float4 lDiffuse;		//bytes 16-31
	float4 lSpecular;		//bytes 32-47

	//for point lights
	float3 lightPosition;	//bytes 48-59

	float padding4;			//bytes 60-63

	//for direction lights
	float3 lightDirection;	//bytes 64-75

	float padding5;		//bytes 76-79
	float4x4 padding6;		//bytes 80-143
	float4x4 padding7;		//bytes 144-207
	float4 padding8;		//bytes 208-223
	float4 padding9;		//bytes 224-239
	float4 padding10;		//bytes 240-255
};

float4 psMain(vertexOutput vout) : SV_TARGET
{
	vout.worldNormal = normalize(vout.worldNormal);

	//light vector form the pixel to the light source
	float3 lightVector = lightPosition.xyz - vout.worldPosition.xyz;

	//attenuation calculation
	float d = length(lightVector);
	float atteunation = 1.0f / (1.0f + 0.001f * d);

	lightVector = normalize(lightVector);

	//view vector from the pixel to the camera pos
	float3 viewVector = cameraPosition - vout.worldPosition.xyz;
	viewVector = normalize(viewVector);

	//the reflected vector of the light vector
	float3 reflectionVector = 2.0f * dot(lightVector, vout.worldNormal.xyz) * vout.worldNormal.xyz - lightVector;
	reflectionVector = normalize(reflectionVector);

	//ambient color of the pixel
	float3 ambientColor = lAmbient.xyz * mAmbient.xyz;

	//diffuse color of the pixel
	float kd = dot(lightVector, vout.worldNormal.xyz);
	kd = clamp(kd, 0.0f, 1.0f);
	float3 diffuseColor = lDiffuse.xyz * mDiffuse.xyz * kd;

	//specular color of the pixel
	float3 specularColor;
	if (kd > 0.0f)
	{
		float ks = dot(reflectionVector, viewVector);
		ks = clamp(ks, 0.0f, 1.0f);
		ks = pow(ks, mShininess);
		specularColor = lSpecular.xyz * mSpecular.xyz * ks;
	}
	else
	{
		specularColor = float3(0.0f, 0.0f, 0.0f);
	}

	//The color of the pixel
	return float4(atteunation * (ambientColor + diffuseColor + specularColor), mAmbient.w);
}