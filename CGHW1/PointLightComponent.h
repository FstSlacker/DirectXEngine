#pragma once
#include "GameComponent.h"
#include "ConstantBuffer.h"
#include <SimpleMath.h>

class PointLightComponent : public GameComponent
{
private:
	struct PointLightDataCbuf
	{
		DirectX::XMFLOAT3 LightColor;
		float LightStrength;
		DirectX::XMFLOAT3 LightPosition;
		float LightRange;
	};

	PSConstantBuffer<PointLightDataCbuf> psLightBuffer;

public:
	DirectX::SimpleMath::Color LightColor;
	float LightStrength;
	float LightRange;

	PointLightComponent(Game* game);

	void Initialize() override;
	void Bind();
	void Draw() override;
};

