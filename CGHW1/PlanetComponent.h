#pragma once
#include "GameComponent.h"
#include "Texture.h"
#include <GeometricPrimitive.h>
#include <Effects.h>
#include <memory>

using namespace DirectX;

class PlanetComponent : public GameComponent
{
private:
	Texture planetTexture;
	std::unique_ptr<GeometricPrimitive> sphere;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
	std::unique_ptr<BasicEffect> effect;

public:
	float OrbitRadius;
	float OrbitSpeed;
	bool IsStar;

	PlanetComponent(Game* game, Vector3 position, float radius, float orbitRad, float orbitSpeed, Texture tex);

	void Update() override;
	void Draw() override;
	void Initialize() override;
	void SetLightDirection(Vector3 dir);
};

