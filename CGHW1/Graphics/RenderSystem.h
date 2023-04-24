#pragma once
#include "RenderSystemFlags.h"
#include "GBuffer.h"
#include "RasterizerState.h"
#include "DepthStencilState.h"
#include "ConstantBuffer.h"
#include "BlendState.h"
#include "Sampler.h"
#include "Material.h"
#include <vector>
#include <map>

class Game;
class MeshComponent;
class LightComponent;
class ParticleSystem;
struct LightData;

class RenderSystem
{
private:
	Game* game;
	GBuffer gBuffer;

	DepthStencil depthStencil;

	std::vector<MeshComponent*> meshes;
	std::vector<LightComponent*> lights;
	std::vector<ParticleSystem*> particleSystems;

	Sampler sampler;

	RasterizerState rStateCullBack;
	RasterizerState rStateCullFront;

	DepthStencilState dsStateOpaque;
	DepthStencilState dsStateLightLessEqual;
	DepthStencilState dsStateLightGreater;
	DepthStencilState dsStateWriteOff;

	BlendState bsLight;

	VSConstantBuffer<TransformCbuf> vsTransformCBuf;
	PSConstantBuffer<TransformCbuf> psTransformCBuf;
	PSConstantBuffer<LightData> psLightCBuf;
	PSConstantBuffer<MaterialCBuf> psMaterialCBuf;

	std::map<VertexShaderOpaque, std::shared_ptr<VertexShader>> opaqueVSs;
	std::map<PixelShaderOpaque, std::shared_ptr<PixelShader>> opaquePSs;

	std::map<VertexShaderLight, std::shared_ptr<VertexShader>> lightVSs;
	std::map<PixelShaderLight, std::shared_ptr<PixelShader>> lightPSs;


	bool InitializeShaders(ID3D11Device* device);

public:
	RenderSystem(Game& game);
	bool Initialize();

	void OpaquePass();
	void LightPass();

	void RegisterMesh(MeshComponent* mesh);
	void RegisterLight(LightComponent* light);
	void RegisterParticleSystem(ParticleSystem* particleSystem);

	GBuffer* GetGBuffer();
};

