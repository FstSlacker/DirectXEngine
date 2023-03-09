#pragma once
#include "GameComponent.h"
#include "ConstantBuffer.h"
#include <SimpleMath.h>

class PointLightComponent : public GameComponent
{
private:
	struct PointLightDataCbuf
	{
		DirectX::XMFLOAT3 AmbientColor;
		float AmbientIntensity;

		DirectX::XMFLOAT3 DiffuseColor;
		float DiffuseIntensity;

		DirectX::XMFLOAT3 CameraPosition;
		float SpecularIntensity;
		float SpecularPower;

		DirectX::XMFLOAT3 LightPosition;
		float LightRange;
	};

	PSConstantBuffer<PointLightDataCbuf> psLightBuffer;

public:
	static DirectX::SimpleMath::Color AmbientColor;
	static float AmbientIntensity;

	DirectX::SimpleMath::Color LightColor;
	float Intensity;
	float SpecularIntensity;
	float SpecularPower;
	float Range;

	PointLightComponent(Game* game);

	void Initialize() override;
	void Bind();
	void Draw() override;
};

