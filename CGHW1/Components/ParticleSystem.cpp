#include "ParticleSystem.h"
#include "../Game.h"
#include <random>
#include <math.h>

#define MAX_PARTICLES_COUNT 1024 * 1024
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
	const UINT maxParticlesCount = MAX_PARTICLES_COUNT;

	int groupSizeX, groupSizeY;
	GetGroupSize(particlesCount, groupSizeX, groupSizeY);

	ParticleSystemParamsCBuf particleSystemParams = {};
	particleSystemParams.CountDeltaTimeGroupDim.x = particlesCount;
	particleSystemParams.CountDeltaTimeGroupDim.y = game->DeltaTime;
	particleSystemParams.CountDeltaTimeGroupDim.z = groupSizeY;

	particleSystemParams.Gravity = Vector4(Gravity);
	particleSystemParams.StartColor = StartColor;
	particleSystemParams.EndColor = EndColor;
	particleSystemParams.Size = Vector4(StartSize, EndSize, game->Display->ClientWidth, game->Display->ClientHeight);


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

	ID3D11ShaderResourceView* srvs[2] = {
		game->RenderSystem.GetGBuffer()->rtWorldPos.GetShaderResourceView(),
		game->RenderSystem.GetGBuffer()->rtNormals.GetShaderResourceView()
	};

	game->Gfx.GetContext()->CSSetShaderResources(0, 2, srvs);

	game->Gfx.GetContext()->CSSetConstantBuffers(0, 1, cbTransform.GetAddressOf());
	game->Gfx.GetContext()->CSSetConstantBuffers(1, 1, cbParticleSystemParams.GetAddressOf());

	game->Gfx.GetContext()->CSSetUnorderedAccessViews(0, 1, uavParticles.GetUAVCounterAddressOf(), &maxParticlesCount);
	game->Gfx.GetContext()->CSSetUnorderedAccessViews(1, 1, uavSortedParticles.GetUAVCounterAddressOf(), &particlesCount);
	game->Gfx.GetContext()->CSSetUnorderedAccessViews(2, 1, uavDeadParticles.GetUAVAppendAddressOf(), &deadParticlesCount);

	csParticlesSimulate->Bind(game->Gfx.GetContext());

	game->Gfx.GetContext()->Dispatch(groupSizeX, groupSizeY, 1);

	UINT prevParticlesCount = particlesCount;

	game->Gfx.GetContext()->CopyStructureCount(rcbCount.Get(), 0, uavDeadParticles.GetUAVAppend());
	rcbCount.Read(game->Gfx.GetContext(), deadParticlesCount);

	particlesCount = MAX_PARTICLES_COUNT - deadParticlesCount;

	if (injectedParticlesCount > 0)
	{
		injectedParticlesCount =
			injectedParticlesCount + prevParticlesCount > MAX_PARTICLES_COUNT ?
			MAX_PARTICLES_COUNT - prevParticlesCount : injectedParticlesCount;

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

		game->Gfx.GetContext()->CSSetUnorderedAccessViews(0, 1, uavParticles.GetUAVCounterAddressOf(), &maxParticlesCount);
		game->Gfx.GetContext()->CSSetUnorderedAccessViews(1, 1, uavSortedParticles.GetUAVAppendAddressOf(), &prevParticlesCount);
		game->Gfx.GetContext()->CSSetUnorderedAccessViews(2, 1, uavDeadParticles.GetUAVAppendAddressOf(), &deadParticlesCount);
		game->Gfx.GetContext()->CSSetUnorderedAccessViews(3, 1, uavParticleInjection.GetUAVAppendAddressOf(), &injectedParticlesCount);

		game->Gfx.GetContext()->Dispatch(groupSizeX, groupSizeY, 1);

		game->Gfx.GetContext()->CopyStructureCount(rcbCount.Get(), 0, uavDeadParticles.GetUAVAppend());
		rcbCount.Read(game->Gfx.GetContext(), deadParticlesCount);

		particlesCount = MAX_PARTICLES_COUNT - deadParticlesCount;

		injectedParticlesCount = 0;
	}

	ID3D11UnorderedAccessView* uavNull = nullptr;
	game->Gfx.GetContext()->CSSetUnorderedAccessViews(0, 1, &uavNull, &counterZero);
	game->Gfx.GetContext()->CSSetUnorderedAccessViews(1, 1, &uavNull, &counterZero);
	game->Gfx.GetContext()->CSSetUnorderedAccessViews(2, 1, &uavNull, &counterZero);
	game->Gfx.GetContext()->CSSetUnorderedAccessViews(3, 1, &uavNull, &counterZero);
}

void ParticleSystem::SortGPU()
{
	ID3D11DeviceContext* context = game->Gfx.GetContext();

	UINT pCountSquared = (UINT)pow(2, ceil(log(MAX_PARTICLES_COUNT) / log(2)));
	pCountSquared = pCountSquared == 1U ? 0U : pCountSquared;

	//Logs::Log("Sq:" + std::to_string(pCountSquared) + " Total:" + std::to_string(particlesCount), false);

	context->CSSetUnorderedAccessViews(0, 1, uavSortedParticles.GetUAVCounterAddressOf(), &pCountSquared);
	csParticlesSort->Bind(context);

	int sizeX, sizeY;
	GetGroupSize(pCountSquared, sizeX, sizeY);

	int stage = 1;
	const int count = pCountSquared;

	int passCount = 0;

	for (int k = 2; k <= count; k <<= 1)
	{
		int stageInd = stage;
		for (int j = k >> 1; j > 0; j >>= 1)
		{
			//----
			cbSortData.Data = Vector4(
				sizeY,
				pCountSquared,
				stage,
				stageInd
			);
			cbSortData.Apply(context);
			context->CSSetConstantBuffers(0, 1, cbSortData.GetAddressOf());

			context->Dispatch(sizeX, sizeY, 1);
			//----
			passCount++;
			stageInd--;
		}
		stage += 1;
	}
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

ParticleSystem::ParticleSystem(Game* game) : GameComponent(game)
{
	this->Name = "ParticleSystem";

	this->MinLifeTime = 1.0f;
	this->MaxLifeTime = 3.0f;

	this->SpawnRate = 100.0f;

	this->Gravity = Vector3(0.0f, -9.8f, 0.0f);

	this->StartColor = Color(1.0f, 1.0f, 0.0f, 1.0f);
	this->EndColor = Color(1.0f, 1.0f, 1.0f, 0.0f);

	this->StartSize = 0.1f;
	this->EndSize = 0.01f;

	this->MinVelocityScale = 1.0f;
	this->MaxVelocityScale = 5.0f;

	this->PositionDistribution = std::make_shared<ConeDistribution>();
	this->VelocityDistribution = std::make_shared<ConeDistribution>();

	game->RenderSystem.RegisterParticleSystem(this);
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
	csParticlesSort = std::make_unique<ComputeShader>(L"./Shaders/CS_BitonicSort.hlsl");

	D3D11_RENDER_TARGET_BLEND_DESC rtBlendDesc = {};
	rtBlendDesc.BlendEnable = true;

	rtBlendDesc.SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
	rtBlendDesc.DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
	rtBlendDesc.BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;

	rtBlendDesc.SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
	rtBlendDesc.DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
	rtBlendDesc.BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;

	rtBlendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;

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

	if (!csParticlesSort->Initialize(game->Gfx.GetDevice()))
		return false;

	if (!uavParticles.Initialize(game->Gfx.GetDevice(), MAX_PARTICLES_COUNT))
		return false;

	if (!uavSortedParticles.Initialize(game->Gfx.GetDevice(), MAX_PARTICLES_COUNT))
		return false;

	if (!uavDeadParticles.Initialize(game->Gfx.GetDevice(), MAX_PARTICLES_COUNT))
		return false;

	if (!uavParticleInjection.Initialize(game->Gfx.GetDevice(), MAX_PARTICLES_INJECTION_COUNT))
		return false;

	if (!cbTransform.Initialize(game->Gfx.GetDevice()))
		return false;

	if (!cbParticleSystemParams.Initialize(game->Gfx.GetDevice()))
		return false;

	if (!cbSortData.Initialize(game->Gfx.GetDevice()))
		return false;

	if (!rcbCount.Initialize(game->Gfx.GetDevice()))
		return false;

	uavParticles.Data = this->particles;
	uavParticles.Apply(game->Gfx.GetContext());

	auto inds = new UINT[MAX_PARTICLES_COUNT];
	auto sortParticles = new Vector2[MAX_PARTICLES_COUNT];

	for (int i = 0; i < MAX_PARTICLES_COUNT; i++)
	{
		inds[i] = i;
		sortParticles[i] = Vector2(i, 1000.0f);
	}

	uavDeadParticles.Data = inds;
	uavDeadParticles.Apply(game->Gfx.GetContext());

	uavSortedParticles.Data = sortParticles;
	uavSortedParticles.Apply(game->Gfx.GetContext());

	delete[] inds;
	delete[] sortParticles;

	deadParticlesCount = MAX_PARTICLES_COUNT;

	//uavSrc = &uavParticlesFirst;
	//uavDest = &uavParticlesSecond;

	return true;
}

void ParticleSystem::Update()
{
	GameComponent::Update();

	Emmit();
	Simulate();
	SortGPU();
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

	game->Gfx.GetContext()->GSSetShaderResources(0, 1, uavParticles.GetShaderResourceViewAddressOf());
	game->Gfx.GetContext()->GSSetShaderResources(1, 1, uavSortedParticles.GetShaderResourceViewAddressOf());
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

		std::string pCountText = "Count: " + std::to_string(particlesCount) + "/" + std::to_string(MAX_PARTICLES_COUNT);

		ImGui::Text(pCountText.c_str());

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

UINT ParticleSystem::GetParticlesCount() const
{
	return this->particlesCount;
}

UINT ParticleSystem::GetMaxParticlesCount() const
{
	return MAX_PARTICLES_COUNT;
}
