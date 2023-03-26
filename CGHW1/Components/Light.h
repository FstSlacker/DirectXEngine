#pragma once
#include "../Graphics/ConstantBuffer.h"
#include "../Graphics/DepthStencil.h"
#include "../Graphics/RenderTarget.h"
#include "../Graphics/Sampler.h"

#include <DirectXMath.h>
#include <DirectXColors.h>
#include <SimpleMath.h>
#include <vector>

#include "GameComponent.h"
#include "Camera.h"

#define MAX_LIGHTS 8

class Game;
class LightComponent;
class PointLightComponent;
class DirectionalLightComponent;
class SpotLightComponent;

class Light : public Bindable
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

		float Intensity; // 4
		float Range; // 4

		float SpotAngle; // 4 
		float ConstantAtt; // 4 

		float LinearAtt; // 4 
		float QuadraticAtt; // 4 

		int LightType; // 4 
		bool Enabled; // 4
	};

	struct LightCbuf
	{
		DirectX::XMFLOAT3 AmbientColor; // 12
		float AmbientIntensity; // 4
		DirectX::XMFLOAT3 CameraPosition; // 12
		float padding;
		LightData Lights[MAX_LIGHTS];
	};

	struct LightVPCbuf
	{
		DirectX::XMFLOAT4 CascadeDistances[4];
		DirectX::XMMATRIX DirectionalLightVP[3];
		DirectX::XMMATRIX LightsVP[MAX_LIGHTS - 1];
		
	};

	PSConstantBuffer<LightCbuf> lightsBuffer;
	PSConstantBuffer<LightVPCbuf> lightsVPBuffer;
	ShadowSampler shadowSampler;

	std::vector<LightComponent*> lights;
	Game* game;


public:
	static DirectX::SimpleMath::Color AmbientColor;
	static float AmbientIntensity;

	Light(Game* game);

	void AddLightComponent(LightComponent* lightComp);
	LightComponent* GetLightComponent(int ind) const;
	int GetLightSourcesCount() const;
	bool Initialize();
	void Bind(ID3D11DeviceContext* context) override;
	void DestroyResources() override;
};

class LightComponent : public GameComponent
{
protected:
	std::vector<Camera*> renderCameras;

	std::vector<std::unique_ptr<RenderTarget>> renderTargets;
	std::unique_ptr<DepthStencil> depthBuffer;

	LightComponent(Game* game);

public:
	Color LightColor;
	float Intensity;

	void Initialize() override;

	const std::vector<Camera*>& GetRenderCameras() const;
	const std::vector< std::unique_ptr<RenderTarget>>& GetRenderTargets() const;
	DepthStencil* GetDepthBuffer() const;

	virtual void DrawGizmos() override;
	virtual void DrawGizmosIcon(Vector3 right, Vector3 up, float scale) override;
};

class PointLightComponent : public LightComponent
{
public:
	float Range;
	float ConstantAttenuation;
	float LinearAttenuation;
	float QuadricAttenuation;

	PointLightComponent(Game* game);

	virtual void DrawGizmos() override;
};

class DirectionalLightComponent : public LightComponent
{
public:
	DirectionalLightComponent(Game* game);

	virtual void DrawGizmos() override;
};

class SpotLightComponent : public LightComponent
{
public:
	float Range;
	float ConeAngle;
	float ConstantAttenuation;
	float LinearAttenuation;
	float QuadricAttenuation;

	SpotLightComponent(Game* game);

	virtual void DrawGizmos() override;
};


