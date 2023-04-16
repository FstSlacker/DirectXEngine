#include "ParticleSystem.h"
#include "../Game.h"
#include <random>
#include <math.h>

#define MAX_PARTICLES_COUNT 100000

void ParticleSystem::CreateRandomParticles()
{
	float size = 10.0f;
	int randDiv = 10000;

	for (int i = 0; i < MAX_PARTICLES_COUNT; i++)
	{
		//particles[i].Position = Vector3(
		//	((float)(rand() % randDiv) * (1.0f / randDiv) * size - size / 2),
		//	((float)(rand() % randDiv) * (1.0f / randDiv) * size - size / 2),
		//	((float)(rand() % randDiv) * (1.0f / randDiv) * size - size / 2)
		//);
		particles[i].Position = Vector3(0.0f, 0.0f, 0.0f);
		particles[i].Velocity = Vector3(
			(float)(rand() % 100) * 0.01 - 0.5f, 
			(float)(rand() % 100) * 0.01 - 0.5f, 
			(float)(rand() % 100) * 0.01 - 0.5f
		) * 5.0f;
		particles[i].Size = (float)(rand() % 100) * 0.001;
		particles[i].LifeTime = (float)(rand() % 100) * 0.1;
		particles[i].Color = Color((float)(rand() % 100) * 0.01, (float)(rand() % 100) * 0.01, (float)(rand() % 100) * 0.01);
	}
}

void ParticleSystem::GetGroupSize(int count, int& sizeX, int& sizeY)
{
	int numGroups = (count % 256 != 0) ? (count / 256 + 1) : (count / 256);
	double r = sqrt(static_cast<double>(numGroups));
	r = ceil(r);

	sizeX = static_cast<int>(r);
	sizeY = static_cast<int>(r);
}

ParticleSystem::ParticleSystem(Game* game) : GameComponent(game)
{
	this->Name = "ParticleSystem";
}

bool ParticleSystem::Initialize()
{
	if (!GameComponent::Initialize())
		return false;

	particles = new Particle[MAX_PARTICLES_COUNT];

	CreateRandomParticles();

	particlesCount = MAX_PARTICLES_COUNT;

	vertexShader = std::make_unique<VertexShader>(L"./Shaders/Particles.hlsl");
	pixelShader = std::make_unique<PixelShader>(L"./Shaders/Particles.hlsl");
	geometryShader = std::make_unique<GeometryShader>(L"./Shaders/Particles.hlsl");

	csSimulate = std::make_unique<ComputeShader>(L"./Shaders/CS_ParticlesSimulate.hlsl");

	if (!vertexShader->Initialize(game->Gfx.GetDevice()))
		return false;

	if (!pixelShader->Initialize(game->Gfx.GetDevice()))
		return false;

	if (!geometryShader->Initialize(game->Gfx.GetDevice()))
		return false;

	if (!csSimulate->Initialize(game->Gfx.GetDevice()))
		return false;

	if (!uavParticlesFirst.Initialize(game->Gfx.GetDevice(), MAX_PARTICLES_COUNT))
		return false;

	if (!uavParticlesSecond.Initialize(game->Gfx.GetDevice(), MAX_PARTICLES_COUNT))
		return false;

	if (!cbTransform.Initialize(game->Gfx.GetDevice()))
		return false;

	if (!cbParticleSystemParams.Initialize(game->Gfx.GetDevice()))
		return false;

	if (!rcbCount.Initialize(game->Gfx.GetDevice()))
		return false;

	uavParticlesFirst.Data = this->particles;
	uavParticlesFirst.Apply(game->Gfx.GetContext());

	uavSrc = &uavParticlesFirst;
	uavDest = &uavParticlesSecond;

	return true;
}

void ParticleSystem::Update()
{
	GameComponent::Update();

	game->Gfx.GetContext()->ClearState();

	int groupSizeX, groupSizeY;
	GetGroupSize(particlesCount, groupSizeX, groupSizeY);

	ParticleSystemParamsCBuf particleSystemParams = {};
	particleSystemParams.CountDeltaTimeGroupDim.x = particlesCount;
	particleSystemParams.CountDeltaTimeGroupDim.y = game->DeltaTime;
	particleSystemParams.CountDeltaTimeGroupDim.z = groupSizeY;

	cbParticleSystemParams.Data = particleSystemParams;
	cbParticleSystemParams.Apply(game->Gfx.GetContext());

	TransformCbuf tData = {};

	tData.ViewPosition = Vector4(Camera::Main->Transform.GetPosition());
	tData.WorldMatrix = XMMatrixTranspose(this->Transform.GetTransformMatrix());
	tData.WorldViewProjMatrix = XMMatrixTranspose(this->Transform.GetTransformMatrix() * Camera::Main->GetViewProjectionMatrix());
	tData.ViewMatrix = XMMatrixTranspose(Camera::Main->GetViewMatrix());
	tData.ProjMatrix = XMMatrixTranspose(Camera::Main->GetProjectionMatrix());

	cbTransform.Data = tData;
	cbTransform.Apply(game->Gfx.GetContext());

	game->Gfx.GetContext()->CSSetConstantBuffers(0, 1, cbTransform.GetAddressOf());
	game->Gfx.GetContext()->CSSetConstantBuffers(1, 1, cbParticleSystemParams.GetAddressOf());

	const UINT counterKeepValue = particlesCount;
	const UINT counterZero = 0;

	game->Gfx.GetContext()->CSSetUnorderedAccessViews(0, 1, uavSrc->GetUnorderedAccessViewAddressOf(), &counterKeepValue);
	game->Gfx.GetContext()->CSSetUnorderedAccessViews(1, 1, uavDest->GetUnorderedAccessViewAddressOf(), &counterZero);

	csSimulate->Bind(game->Gfx.GetContext());

	game->Gfx.GetContext()->Dispatch(groupSizeX, groupSizeY, 1);

	ID3D11UnorderedAccessView* uavNull = nullptr;
	game->Gfx.GetContext()->CSSetUnorderedAccessViews(0, 1, &uavNull, &counterZero);
	game->Gfx.GetContext()->CSSetUnorderedAccessViews(1, 1, &uavNull, &counterZero);

	game->Gfx.GetContext()->CopyStructureCount(rcbCount.Get(), 0, uavDest->GetUnorderedAccessView());

	UINT count;
	rcbCount.Read(game->Gfx.GetContext(), count);

	particlesCount = count;

	//Logs::Log(std::to_string(count), false);

	//Swap
	auto tmp = uavSrc;
	uavSrc = uavDest;
	uavDest = tmp;
	//---

}

void ParticleSystem::Bind()
{
	vertexShader->Bind(game->Gfx.GetContext());
	pixelShader->Bind(game->Gfx.GetContext());
	geometryShader->Bind(game->Gfx.GetContext());

	game->Gfx.GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	TransformCbuf tData = {};

	tData.ViewPosition = Vector4(Camera::Main->Transform.GetPosition());
	tData.WorldMatrix = XMMatrixTranspose(this->Transform.GetTransformMatrix());
	tData.WorldViewProjMatrix = XMMatrixTranspose(this->Transform.GetTransformMatrix() * Camera::Main->GetViewProjectionMatrix());
	tData.ViewMatrix = XMMatrixTranspose(Camera::Main->GetViewMatrix());
	tData.ProjMatrix = XMMatrixTranspose(Camera::Main->GetProjectionMatrix());

	cbTransform.Data = tData;
	cbTransform.Apply(game->Gfx.GetContext());

	game->Gfx.GetContext()->GSSetConstantBuffers(0, 1, cbTransform.GetAddressOf());
	game->Gfx.GetContext()->GSSetShaderResources(0, 1, uavSrc->GetShaderResourceViewAddressOf());
}

void ParticleSystem::Draw()
{
	game->Gfx.GetContext()->Draw(particlesCount, 0);
}
