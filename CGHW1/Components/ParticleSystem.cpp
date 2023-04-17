#include "ParticleSystem.h"
#include "../Game.h"
#include <random>
#include <math.h>

#define MAX_PARTICLES_COUNT 100000
#define MAX_PARTICLES_INJECTION_COUNT 1000

void ParticleSystem::Emmit()
{
	spawnCounter += SpawnRate * game->DeltaTime;

	int count = (int)spawnCounter;

	spawnCounter = spawnCounter - (int)spawnCounter;

	for (int i = 0; i < count; i++)
	{
		Particle p = {};
		p.Color = Color(1.0f, 0.0f, 0.0f);

		Vector3 v = VelocityDistribution->GetRandomSurfacePoint();
		v.Normalize();

		p.Velocity = v * (((float)rand() / (float)(RAND_MAX)) * (MaxVelocityScale - MinVelocityScale) + MinVelocityScale);

		p.Position = Transform.GetPosition() + PositionDistribution->GetRandomInnerPoint();
		p.LifeTime = ((float)rand() / (float)(RAND_MAX)) * (MaxLifeTime - MinLifeTime) + MinLifeTime;
		p.Color = StartColor;
		p.Size = StartSize;

		AddParticle(p);
	}
}

void ParticleSystem::Simulate()
{
	game->Gfx.GetContext()->ClearState();

	const UINT counterKeepValue = -1;
	const UINT counterZero = 0;

	int groupSizeX, groupSizeY;
	GetGroupSize(particlesCount, groupSizeX, groupSizeY);

	ParticleSystemParamsCBuf particleSystemParams = {};
	particleSystemParams.CountDeltaTimeGroupDim.x = particlesCount;
	particleSystemParams.CountDeltaTimeGroupDim.y = game->DeltaTime;
	particleSystemParams.CountDeltaTimeGroupDim.z = groupSizeY;

	particleSystemParams.Gravity = Vector4(Gravity);
	particleSystemParams.StartColor = StartColor;
	particleSystemParams.EndColor = EndColor;
	particleSystemParams.Size = Vector4(StartSize, EndSize, 0.0f, 0.0f);

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

	game->Gfx.GetContext()->CSSetUnorderedAccessViews(0, 1, uavSrc->GetUnorderedAccessViewAddressOf(), &particlesCount);
	game->Gfx.GetContext()->CSSetUnorderedAccessViews(1, 1, uavDest->GetUnorderedAccessViewAddressOf(), &counterZero);

	csParticlesSimulate->Bind(game->Gfx.GetContext());

	game->Gfx.GetContext()->Dispatch(groupSizeX, groupSizeY, 1);

	game->Gfx.GetContext()->CopyStructureCount(rcbCount.Get(), 0, uavDest->GetUnorderedAccessView());
	rcbCount.Read(game->Gfx.GetContext(), particlesCount);

	if (injectedParticlesCount > 0)
	{
		injectedParticlesCount =
			injectedParticlesCount + particlesCount > MAX_PARTICLES_COUNT ?
			MAX_PARTICLES_COUNT - particlesCount : injectedParticlesCount;

		GetGroupSize(injectedParticlesCount, groupSizeX, groupSizeY);

		particleSystemParams.CountDeltaTimeGroupDim.x = injectedParticlesCount;
		particleSystemParams.CountDeltaTimeGroupDim.y = game->DeltaTime;
		particleSystemParams.CountDeltaTimeGroupDim.z = groupSizeY;

		cbParticleSystemParams.Data = particleSystemParams;
		cbParticleSystemParams.Apply(game->Gfx.GetContext());

		csParticlesInject->Bind(game->Gfx.GetContext());

		game->Gfx.GetContext()->CSSetConstantBuffers(0, 1, cbTransform.GetAddressOf());
		game->Gfx.GetContext()->CSSetConstantBuffers(1, 1, cbParticleSystemParams.GetAddressOf());

		uavParticleInjection.Data = injectedParticles;
		uavParticleInjection.Apply(game->Gfx.GetContext());

		game->Gfx.GetContext()->CSSetUnorderedAccessViews(0, 1, uavParticleInjection.GetUnorderedAccessViewAddressOf(), &injectedParticlesCount);
		game->Gfx.GetContext()->CSSetUnorderedAccessViews(1, 1, uavDest->GetUnorderedAccessViewAddressOf(), &particlesCount);

		game->Gfx.GetContext()->Dispatch(groupSizeX, groupSizeY, 1);

		game->Gfx.GetContext()->CopyStructureCount(rcbCount.Get(), 0, uavDest->GetUnorderedAccessView());
		rcbCount.Read(game->Gfx.GetContext(), particlesCount);

		injectedParticlesCount = 0;
	}

	ID3D11UnorderedAccessView* uavNull = nullptr;
	game->Gfx.GetContext()->CSSetUnorderedAccessViews(0, 1, &uavNull, &counterZero);
	game->Gfx.GetContext()->CSSetUnorderedAccessViews(1, 1, &uavNull, &counterZero);

	SwapParticlesBuffers();
}

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

void ParticleSystem::SwapParticlesBuffers()
{
	auto tmp = uavSrc;
	uavSrc = uavDest;
	uavDest = tmp;
}

ParticleSystem::ParticleSystem(Game* game) : GameComponent(game)
{
	this->Name = "ParticleSystem";

	this->MinLifeTime = 1.0f;
	this->MaxLifeTime = 3.0f;

	this->SpawnRate = 100.0f;

	this->Gravity = Vector3(0.0f, -9.8f, 0.0f);

	this->StartColor = Color(1.0f, 0.0f, 0.0f, 1.0f);
	this->EndColor = Color(1.0f, 1.0f, 1.0f, 0.0f);

	this->StartSize = 0.1f;
	this->EndSize = 0.01f;

	this->MinVelocityScale = 1.0f;
	this->MaxVelocityScale = 5.0f;

	this->PositionDistribution = std::make_shared<ConeDistribution>();
	this->VelocityDistribution = std::make_shared<ConeDistribution>();
}

bool ParticleSystem::Initialize()
{
	if (!GameComponent::Initialize())
		return false;

	particles = new Particle[MAX_PARTICLES_COUNT];
	injectedParticles = new Particle[MAX_PARTICLES_INJECTION_COUNT];

	particlesCount = 0;
	injectedParticlesCount = 0;

	vertexShader = std::make_unique<VertexShader>(L"./Shaders/Particles.hlsl");
	pixelShader = std::make_unique<PixelShader>(L"./Shaders/Particles.hlsl");
	geometryShader = std::make_unique<GeometryShader>(L"./Shaders/Particles.hlsl");

	csParticlesSimulate = std::make_unique<ComputeShader>(L"./Shaders/CS_ParticlesSimulate.hlsl");
	csParticlesInject = std::make_unique<ComputeShader>(L"./Shaders/CS_ParticlesInject.hlsl");

	D3D11_RENDER_TARGET_BLEND_DESC rtBlendDesc = {};
	rtBlendDesc.BlendEnable = true;

	rtBlendDesc.SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
	rtBlendDesc.DestBlend = D3D11_BLEND::D3D11_BLEND_ONE;
	rtBlendDesc.BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;

	rtBlendDesc.SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
	rtBlendDesc.DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
	rtBlendDesc.BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;

	rtBlendDesc.RenderTargetWriteMask = 0x0f;

	D3D11_BLEND_DESC blendDesc = {};
	blendDesc.RenderTarget[0] = rtBlendDesc;

	if (!blendState.Initialize(game->Gfx.GetDevice(), blendDesc))
		return false;

	if (!vertexShader->Initialize(game->Gfx.GetDevice()))
		return false;

	if (!pixelShader->Initialize(game->Gfx.GetDevice()))
		return false;

	if (!geometryShader->Initialize(game->Gfx.GetDevice()))
		return false;

	if (!csParticlesSimulate->Initialize(game->Gfx.GetDevice()))
		return false;

	if (!csParticlesInject->Initialize(game->Gfx.GetDevice()))
		return false;

	if (!uavParticlesFirst.Initialize(game->Gfx.GetDevice(), MAX_PARTICLES_COUNT))
		return false;

	if (!uavParticlesSecond.Initialize(game->Gfx.GetDevice(), MAX_PARTICLES_COUNT))
		return false;

	if (!uavParticleInjection.Initialize(game->Gfx.GetDevice(), MAX_PARTICLES_INJECTION_COUNT))
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

	Emmit();
	Simulate();
}

void ParticleSystem::Bind()
{
	blendState.Bind(game->Gfx.GetContext());

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

void ParticleSystem::DrawGui()
{
	GameComponent::DrawGui();

	if (ImGui::CollapsingHeader("ParticleSystem", ImGuiTreeNodeFlags_DefaultOpen))
	{
		const char* items[] = { "Point", "Linear",  "Sphere", "Cone", "Box" };
		int currentItem = 0;

		ImGui::DragFloat("SpawnRate", &SpawnRate);

		if (ImGui::TreeNodeEx("LifeTime", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::DragFloat("Min###MinLifeTime", &MinLifeTime);
			ImGui::DragFloat("Max###MaxLifeTime", &MaxLifeTime);

			ImGui::TreePop();
		}

		if (ImGui::TreeNodeEx("Position", ImGuiTreeNodeFlags_DefaultOpen))
		{
			currentItem = static_cast<int>(this->PositionDistribution->GetDistributionType());

			if (ImGui::Combo("Distribution###DistributionTypePosition", &currentItem, items, IM_ARRAYSIZE(items)))
			{
				switch (static_cast<DistributionType>(currentItem))
				{
					case DistributionType::Point:
					{
						PositionDistribution = std::make_shared<PointDistribution>();
						break;
					}
					case DistributionType::Linear:
					{
						PositionDistribution = std::make_shared<LinearDistribution>();
						break;
					}
					case DistributionType::Sphere:
					{
						PositionDistribution = std::make_shared<SphereDistribution>();
						break;
					}
					case DistributionType::Cone:
					{
						PositionDistribution = std::make_shared<ConeDistribution>();
						break;
					}
					case DistributionType::Box:
					{
						PositionDistribution = std::make_shared<BoxDistribution>();
						break;
					}
				}
			}

			PositionDistribution->DrawGui();

			ImGui::TreePop();
		}

		if (ImGui::TreeNodeEx("Color", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::ColorEdit4("StartColor", (float*)(&StartColor));
			ImGui::ColorEdit4("EndColor", (float*)(&EndColor));

			ImGui::TreePop();
		}

		if (ImGui::TreeNodeEx("Size", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::DragFloat("StartSize", &StartSize);
			ImGui::DragFloat("EndSize", &EndSize);

			ImGui::TreePop();
		}

		if(ImGui::TreeNodeEx("Velocity", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::DragFloat("Min###MinVelocityScale", &MinVelocityScale);
			ImGui::DragFloat("Max###MaxVelocityScale", &MaxVelocityScale);
			ImGui::DragFloat3("Gravity", (float*)(&Gravity));

			currentItem = static_cast<int>(this->VelocityDistribution->GetDistributionType());

			if (ImGui::Combo("Distribution###DistributionTypeVelocity", &currentItem, items, IM_ARRAYSIZE(items)))
			{

				switch (static_cast<DistributionType>(currentItem))
				{
					case DistributionType::Point:
					{
						VelocityDistribution = std::make_shared<PointDistribution>();
						break;
					}
					case DistributionType::Linear:
					{
						VelocityDistribution = std::make_shared<LinearDistribution>();
						break;
					}
					case DistributionType::Sphere:
					{
						VelocityDistribution = std::make_shared<SphereDistribution>();
						break;
					}
					case DistributionType::Cone:
					{
						VelocityDistribution = std::make_shared<ConeDistribution>();
						break;
					}
					case DistributionType::Box:
					{
						VelocityDistribution = std::make_shared<BoxDistribution>();
						break;
					}
				}
			}

			VelocityDistribution->DrawGui();

			ImGui::TreePop();
		}
	}
}

bool ParticleSystem::AddParticle(Particle& p)
{
	if (injectedParticlesCount == MAX_PARTICLES_INJECTION_COUNT)
		return false;

	p.MaxLifeTime = p.LifeTime;

	injectedParticles[injectedParticlesCount++] = p;

	return true;
}
