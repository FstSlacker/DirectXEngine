#include "../Game.h"
#include "RenderSystem.h"

bool RenderSystem::InitializeShaders(ID3D11Device* device)
{
    opaqueVSs[VertexShaderOpaque::Default] = std::make_shared<VertexShader>(L"./Shaders/Deferred.hlsl");
    opaquePSs[PixelShaderOpaque::Default] = std::make_shared<PixelShader>(L"./Shaders/Deferred.hlsl");

    lightVSs[VertexShaderLight::ScreenQuad] = std::make_shared<VertexShader>(L"./Shaders/VS_ScreenAlignedQuad.hlsl");
    lightPSs[PixelShaderLight::AmbientLight] = std::make_shared<PixelShader>(L"./Shaders/PS_AmbientLight.hlsl");

    for (auto vs : opaqueVSs)
    {
        if (!vs.second->Initialize(device))
            return false;
    }

    for (auto ps : opaquePSs)
    {
        if (!ps.second->Initialize(device))
            return false;
    }

    for (auto vs : lightVSs)
    {
        if (!vs.second->Initialize(device))
            return false;
    }

    for (auto ps : lightPSs)
    {
        if (!ps.second->Initialize(device))
            return false;
    }

    return true;
}

RenderSystem::RenderSystem(Game& game)
{
    this->game = &game;
}

bool RenderSystem::Initialize()
{
    ID3D11Device* device = game->Gfx.GetDevice();

    if (!gBuffer.Initialize(device, game->Display->ClientWidth, game->Display->ClientHeight))
        return false;

    if (!depthStencil.Initialize(device, game->Display->ClientWidth, game->Display->ClientHeight))
        return false;

    D3D11_DEPTH_STENCIL_DESC dsLessEqualDesc = {};
    dsLessEqualDesc.DepthEnable = true;
    dsLessEqualDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
    dsLessEqualDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

    if (!dsStateLessEqual.Initialize(device, dsLessEqualDesc))
        return false;

    D3D11_DEPTH_STENCIL_DESC dsGreaterDesc = {};
    dsGreaterDesc.DepthEnable = true;
    dsGreaterDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
    dsGreaterDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_GREATER;

    if (!dsStateGreater.Initialize(device, dsGreaterDesc))
        return false;

    D3D11_DEPTH_STENCIL_DESC dsOffDesc = {};
    dsOffDesc.DepthEnable = false;

    if (!dsStateOff.Initialize(device, dsOffDesc))
        return false;

    if (!sampler.Initialize(device))
        return false;

    CD3D11_RASTERIZER_DESC rastCullBackDesc = {};
    rastCullBackDesc.CullMode = D3D11_CULL_BACK;
    rastCullBackDesc.FillMode = D3D11_FILL_SOLID;

    if (!rStateCullBack.Initialize(device, rastCullBackDesc))
        return false;

    CD3D11_RASTERIZER_DESC rastCullFrontDesc = {};
    rastCullFrontDesc.CullMode = D3D11_CULL_FRONT;
    rastCullFrontDesc.FillMode = D3D11_FILL_SOLID;

    if (!rStateCullFront.Initialize(device, rastCullFrontDesc))
        return false;

    HRESULT hr = lightCBuf.Initialize(device);
    if (FAILED(hr))
    {
        Logs::LogError(hr, "Failed to init LightCBuf");
        return false;
    }

    if (!InitializeShaders(device))
        return false;

    return true;
}

void RenderSystem::OpaquePass()
{
    ID3D11DeviceContext* context = game->Gfx.GetContext();

    //context->ClearState();
    gBuffer.rtDiffuse.Clear(context, Color(0.0f, 0.0f, 0.0f, 0.0f));
    gBuffer.rtEmissive.Clear(context, Color(0.0f, 0.0f, 0.0f, 0.0f));
    gBuffer.rtNormals.Clear(context, Color(0.0f, 0.0f, 0.0f, 0.0f));
    gBuffer.rtWorldPos.Clear(context, Color(0.0f, 0.0f, 0.0f, 0.0f));
    depthStencil.Clear(context);

    rStateCullBack.Bind(context);
    dsStateLessEqual.Bind(context);

    ID3D11RenderTargetView* rts[8] = {
        gBuffer.rtDiffuse.GetRenderTargetView(),
        gBuffer.rtEmissive.GetRenderTargetView(),
        gBuffer.rtNormals.GetRenderTargetView(),
        gBuffer.rtWorldPos.GetRenderTargetView(),
        nullptr,
        nullptr,
        nullptr,
        nullptr
    };

    context->OMSetRenderTargets(8, rts, depthStencil.GetView());
    context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    opaqueVSs[VertexShaderOpaque::Default]->Bind(context);
    opaquePSs[PixelShaderOpaque::Default]->Bind(context);

    sampler.Bind(context, 0);

    D3D11_VIEWPORT viewport = {};
    viewport.Width = static_cast<float>(game->Display->ClientWidth);
    viewport.Height = static_cast<float>(game->Display->ClientHeight);
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.MinDepth = 0;
    viewport.MaxDepth = 1.0f;

    context->RSSetViewports(1, &viewport);

    for (auto m : meshes)
    {
        //Bind ps resources
        if (m->GetMaterial()->DiffuseTexture != nullptr) m->GetMaterial()->DiffuseTexture->Bind(context, 0);
        if (m->GetMaterial()->NormalMapTexture != nullptr) m->GetMaterial()->NormalMapTexture->Bind(context, 1);
        if (m->GetMaterial()->SpecularMapTexture != nullptr) m->GetMaterial()->SpecularMapTexture->Bind(context, 2);

        //Bind mesh
        m->Bind();

        //Draw mesh
        m->Draw();
    }
}

void RenderSystem::LightPass()
{
    ID3D11DeviceContext* context = game->Gfx.GetContext();

    context->ClearState();
    //ID3D11ShaderResourceView* gBufTexs[8] = {
    //    gBuffer.rtDiffuse.GetShaderResourceView(),
    //    gBuffer.rtEmissive.GetShaderResourceView(),
    //    gBuffer.rtNormals.GetShaderResourceView(),
    //    gBuffer.rtWorldPos.GetShaderResourceView()
    //};

    gBuffer.rtDiffuse.BindAsTexture(context, 0);
    gBuffer.rtEmissive.BindAsTexture(context, 1);
    gBuffer.rtNormals.BindAsTexture(context, 2);
    gBuffer.rtWorldPos.BindAsTexture(context, 3);

    //context->PSSetShaderResources(0, 4, gBufTexs);

    D3D11_VIEWPORT viewport = {};
    viewport.Width = static_cast<float>(game->Display->ClientWidth);
    viewport.Height = static_cast<float>(game->Display->ClientHeight);
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.MinDepth = 0;
    viewport.MaxDepth = 1.0f;

    context->RSSetViewports(1, &viewport);

    //Set constant buffers
    sampler.Bind(context);

    //??? depth stencil and rt (clear rt)
    game->Gfx.BindRenderTarget();

    for (auto light : lights)
    {
        if (light->GetLightType() == LightType::Directional || light->GetLightType() == LightType::Ambient)
        {
            //Set constant buffers
            lightCBuf.SetSlot(1);
            lightCBuf.Data = light->GetLightData();
            lightCBuf.ApplyChanges(context);
            lightCBuf.Bind(context);

            rStateCullBack.Bind(context);
            dsStateLessEqual.Bind(context);

            lightVSs[VertexShaderLight::ScreenQuad]->Bind(context);
            lightPSs[light->GetPixelShaderLightType()]->Bind(context);

            context->IASetInputLayout(nullptr);
            context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

            context->IASetIndexBuffer(nullptr, DXGI_FORMAT_R32_UINT, 0);
            context->Draw(4, 0);
        }
    }
}

void RenderSystem::RegisterMesh(MeshComponent* mesh)
{
    meshes.push_back(mesh);
}

void RenderSystem::RegisterLight(LightComponent* light)
{
    lights.push_back(light);
}

GBuffer* RenderSystem::GetGBuffer()
{
    return &gBuffer;
}
