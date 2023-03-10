#pragma once
#include "ConstantBuffer.h"
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <SimpleMath.h>
#include "PointLightComponent.h"

#define MAX_LIGHTS 8

class Game;

class Light
{
private:
	enum class LightType : int
	{
		Directional,
		Point,
		Spot
	};
	struct LightData
	{
		DirectX::XMFLOAT4 Position; // 16 
		DirectX::XMFLOAT4 Direction; // 16 
		DirectX::XMFLOAT4 Color; // 16 

		float SpotAngle; // 4 
		float ConstantAtt; // 4 
		float LinearAtt; // 4 
		float QuadraticAtt; // 4 

		int LightType; // 4 
		bool Enabled; // 4 
		float padding[2]; // 8
	};

	struct LightCbuf
	{
		DirectX::XMFLOAT3 AmbientColor; // 12
		float AmbientIntensity; // 4
		DirectX::XMFLOAT3 CameraPosition; // 12
		float padding;
		LightData Lights[MAX_LIGHTS];
	};

	PSConstantBuffer<LightCbuf> lightsBuffer;
	std::vector<PointLightComponent*> lights;
	Game* game;


public:
	static DirectX::SimpleMath::Color AmbientColor;
	static float AmbientIntensity;

	Light(Game* game);

	void AddLightComponent(PointLightComponent* lightComp);
	PointLightComponent* GetLightComponent(int ind) const;
	int GetLightSourcesCount() const;
	bool Initialize();
	void Bind();
};

