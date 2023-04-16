#pragma once
#include "GameComponent.h"
#include <SimpleMath.h>
#include "../Graphics/StructuredBuffer.h";
#include "../Graphics/ConstantBuffer.h"
#include "../Graphics/Shaders.h"
#include "../Graphics/BlendState.h"

using namespace DirectX::SimpleMath;

struct Particle
{
public:
	Vector3 Position;
	Vector3 Velocity;
	Color Color;
	float Size;
	float LifeTime;
	float MaxLifeTime;
};

class ParticleSystem : public GameComponent
{
private:
	struct ParticleSystemParamsCBuf
	{
		XMFLOAT4 CountDeltaTimeGroupDim;
		XMFLOAT4 Gravity;
		XMFLOAT4 Size;
		XMFLOAT4 StartColor;
		XMFLOAT4 EndColor;
	};

	BlendState blendState;

	UAVStructuredBuffer<Particle> uavParticlesFirst;
	UAVStructuredBuffer<Particle> uavParticlesSecond;
	UAVStructuredBuffer<Particle> uavParticleInjection;

	UAVStructuredBuffer<Particle>* uavSrc;
	UAVStructuredBuffer<Particle>* uavDest;

	ConstantBuffer<TransformCbuf> cbTransform;
	ConstantBuffer<ParticleSystemParamsCBuf> cbParticleSystemParams;

	RConstantBuffer<UINT> rcbCount;

	std::unique_ptr<VertexShader> vertexShader;
	std::unique_ptr<PixelShader> pixelShader;
	std::unique_ptr<GeometryShader> geometryShader;

	std::unique_ptr<ComputeShader> csParticlesSimulate;
	std::unique_ptr<ComputeShader> csParticlesInject;

	Particle* particles;
	Particle* injectedParticles;

	UINT particlesCount = 0;
	UINT injectedParticlesCount = 0;

	float spawnCounter = 0.0f;

	void Emmit();
	void Simulate();
	void CreateRandomParticles();
	void GetGroupSize(int count, int& sizeX, int& sizeY);
	void SwapParticlesBuffers();

public:

	float MinLifeTime;
	float MaxLifeTime;
	float SpawnRate;
	Vector3 Gravity;
	Color StartColor;
	Color EndColor;
	float StartSize;
	float EndSize;

	ParticleSystem(Game* game);

	bool Initialize() override;
	void Update() override;
	void Bind() override;
	void Draw() override;

	void DrawGui() override;

	bool AddParticle(Particle& p);
};

