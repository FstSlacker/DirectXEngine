#pragma once
#include "GameComponent.h"
#include "Texture.h"
#include <GeometricPrimitive.h>
#include <Effects.h>
#include <memory>

using namespace DirectX;

struct PlanetInfo
{
	std::string Name;
	float Mass;
	float Diameter;
	float Density;
	float Gravity;
	float LengthOfDay;
	float OrbitRadius;
	float OrbitPeriod;
	float RotationPeriod;
	float ObliquityToOrbit;
};

class PlanetComponent : public GameComponent
{
private:
	Texture planetTexture;
	std::unique_ptr<GeometricPrimitive> sphere;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
	std::unique_ptr<BasicEffect> effect;

	void ReloadGraphics(bool isLighting);

public:
	PlanetInfo Info;
	bool IsStar;

	PlanetComponent(Game* game, Vector3 position, PlanetInfo info, Texture tex);

	void Update() override;
	void Draw() override;
	void Initialize() override;
	void SetLightDirection(Vector3 dir);
	void SetLighting(bool isActive);
};

