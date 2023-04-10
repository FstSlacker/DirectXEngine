#pragma once
#include "../Graphics/RenderSystemFlags.h"

#include <DirectXMath.h>
#include <DirectXColors.h>
#include <SimpleMath.h>
#include <vector>

#include "GameComponent.h"
#include "Mesh.h"

class Game;
class LightComponent;
class PointLightComponent;
class DirectionalLightComponent;
class SpotLightComponent;

enum class LightType
{
	Ambient,
	Directional,
	Point,
	Spot
};

#pragma pack(push, 4)
struct LightData
{
	Vector4 Position;
	Vector4 Direction;
	Vector4 Params;
	Color Color;
};
#pragma pack(pop)


class LightComponent : public GameComponent
{
protected:
	std::shared_ptr<Mesh> mesh = nullptr;

	LightComponent(Game* game);

public:
	Color LightColor;
	float Intensity;

	bool Initialize() override;

	virtual LightType GetLightType() const = 0;
	virtual PixelShaderLight GetPixelShaderLightType() const = 0;
	virtual LightData GetLightData() const = 0;
	Mesh* GetMesh() const;

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

	LightType GetLightType() const override;
	PixelShaderLight GetPixelShaderLightType() const override;
	LightData GetLightData() const override;

	virtual void DrawGizmos() override;
};

class DirectionalLightComponent : public LightComponent
{
public:
	DirectionalLightComponent(Game* game);

	LightType GetLightType() const override;
	PixelShaderLight GetPixelShaderLightType() const override;
	LightData GetLightData() const override;

	virtual void DrawGizmos() override;
};

class AmbientLightComponent : public LightComponent
{
public:
	AmbientLightComponent(Game* game);

	LightType GetLightType() const override;
	PixelShaderLight GetPixelShaderLightType() const override;
	LightData GetLightData() const override;

	virtual void DrawGizmos() override;
	virtual void DrawGizmosIcon(Vector3 right, Vector3 up, float scale) override;
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

	LightType GetLightType() const override;
	PixelShaderLight GetPixelShaderLightType() const override;
	LightData GetLightData() const override;

	virtual void DrawGizmos() override;
};


