#pragma once
#include "GameComponent.h"
#include <SimpleMath.h>
#include "../Graphics/StructuredBuffer.h";
#include "../Graphics/ConstantBuffer.h"
#include "../Graphics/Shaders.h"

using namespace DirectX::SimpleMath;

struct Particle
{
public:
	Vector3 Position;
	Vector3 Velocity;
	Color Color;
	float Size;
	float LifeTime;
};

class ParticleSystem : public GameComponent
{
private:
	struct ParticleSystemParamsCBuf
	{
		XMFLOAT4 CountDeltaTimeGroupDim;
	};

	Particle* particles;

	UAVStructuredBuffer<Particle> uavParticlesFirst;
	UAVStructuredBuffer<Particle> uavParticlesSecond;

	UAVStructuredBuffer<Particle>* uavSrc;
	UAVStructuredBuffer<Particle>* uavDest;

	ConstantBuffer<TransformCbuf> cbTransform;
	ConstantBuffer<ParticleSystemParamsCBuf> cbParticleSystemParams;

	RConstantBuffer<UINT> rcbCount;

	std::unique_ptr<VertexShader> vertexShader;
	std::unique_ptr<PixelShader> pixelShader;
	std::unique_ptr<GeometryShader> geometryShader;

	std::unique_ptr<ComputeShader> csSimulate;

	UINT particlesCount = 0;

	void CreateRandomParticles();
	void GetGroupSize(int count, int& sizeX, int& sizeY);

public:
	ParticleSystem(Game* game);

	bool Initialize() override;
	void Update() override;
	void Bind() override;
	void Draw() override;
};

