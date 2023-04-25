#include "../Game.h"
#include "RenderSystem.h"

bool RenderSystem::InitializeShaders(ID3D11Device* device)
{
    opaqueVSs[VertexShaderOpaque::Default] = std::make_shared<VertexShader>(L"./Shaders/Deferred.hlsl");
    opaquePSs[PixelShaderOpaque::Default] = std::make_shared<PixelShader>(L"./Shaders/Deferred.hlsl");

    lightVSs[VertexShaderLight::ScreenQuad] = std::make_shared<VertexShader>(L"./Shaders/VS_ScreenAlignedQuad.hlsl");
    lightVSs[VertexShaderLight::Mesh] = std::make_shared<VertexShader>(L"./Shaders/VS_DeferredLight.hlsl");

    lightPSs[PixelShaderLight::AmbientLight] = std::make_shared<PixelShader>(L"./Shaders/PS_AmbientLight.hlsl");
    lightPSs[PixelShaderLight::DirectionalLight] = std::make_shared<PixelShader>(L"./Shaders/PS_DirectionalLight.hlsl");
    lightPSs[PixelShaderLight::PointLight] = std::make_shared<PixelShader>(L"./Shaders/PS_PointLight.hlsl");
    lightPSs[PixelShaderLight::SpotLight] = std::make_shared<PixelShader>(L"./Shaders/PS_SpotLight.hlsl");

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

    D3D11_DEPTH_STENCIL_DESC dsOpaqueDesc = {};
    dsOpaqueDesc.DepthEnable = true;
    dsOpaqueDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
    dsOpaqueDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

    if (!dsStateOpaque.Initialize(device, dsOpaqueDesc))
        return false;

    D3D11_DEPTH_STENCIL_DESC dsLessEqualDesc = {};
    dsLessEqualDesc.DepthEnable = true;
    dsLessEqualDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ZERO;
    dsLessEqualDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

    if (!dsStateLightLessEqual.Initialize(device, dsLessEqualDesc))
        return false;

    D3D11_DEPTH_STENCIL_DESC dsGreaterDesc = {};
    dsGreaterDesc.DepthEnable = true;
    dsGreaterDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ZERO;
    dsGreaterDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_GREATER;

    if (!dsStateLightGreater.Initialize(device, dsGreaterDesc))
        return false;

    D3D11_DEPTH_STENCIL_DESC dsWriteOffDesc = {};
    dsWriteOffDesc.DepthEnable = true;
    dsWriteOffDesc.StencilEnable = false;
    dsWriteOffDesc.DepthFunc = D3D11_COMPARISON_LESS;
    dsWriteOffDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

    if (!dsStateWriteOff.Initialize(device, dsWriteOffDesc))
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

    if (!vsTransformCBuf.Initialize(device))
        return false;

    if (!psLightCBuf.Initialize(device))
        return false;

    if (!psTransformCBuf.Initialize(device))
        return false;

    if (!psMaterialCBuf.Initialize(device))
        return false;

    D3D11_RENDER_TARGET_BLEND_DESC rtBlendDesc1 = {};
    rtBlendDesc1.BlendEnable = true;
    rtBlendDesc1.SrcBlend = D3D11_BLEND::D3D11_BLEND_ONE;
    rtBlendDesc1.DestBlend = D3D11_BLEND::D3D11_BLEND_ONE;
    rtBlendDesc1.SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
    rtBlendDesc1.DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
    rtBlendDesc1.BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
    rtBlendDesc1.BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
    rtBlendDesc1.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;

    D3D11_BLEND_DESC blendDesc = {};
    blendDesc.AlphaToCoverageEnable = false;
    blendDesc.IndependentBlendEnable = false;
    blendDesc.RenderTarget[0] = rtBlendDesc1;

    if (!bsLight.Initialize(device, blendDesc))
        return false;

    if (!InitializeShaders(device))
        return false;

    return true;
}

void RenderSystem::OpaquePass()
{
    ID3D11DeviceContext* context = game->Gfx.GetContext();

    context->ClearState();

    gBuffer.rtDiffuse.Clear(context, Color(0.0f, 0.0f, 0.0f, 0.0f));
    gBuffer.rtEmissive.Clear(context, Color(0.0f, 0.0f, 0.0f, 0.0f));
    gBuffer.rtNormals.Clear(context, Color(0.0f, 0.0f, 0.0f, 0.0f));
    gBuffer.rtSpecular.Clear(context, Color(0.0f, 0.0f, 0.0f, 0.0f));
    gBuffer.rtWorldPos.Clear(context, Color(0.0f, 0.0f, 0.0f, 0.0f));
    depthStencil.Clear(context);

    rStateCullBack.Bind(context);
    dsStateOpaque.Bind(context);

    ID3D11RenderTargetView* rts[8] = {
        gBuffer.rtDiffuse.GetRenderTargetView(),
        gBuffer.rtEmissive.GetRenderTargetView(),
        gBuffer.rtNormals.GetRenderTargetView(),
        gBuffer.rtSpecular.GetRenderTargetView(),
        gBuffer.rtWorldPos.GetRenderTargetView(),
        nullptr,
        nullptr,
        nullptr
    };

    context->OMSetRenderTargets(8, rts, depthStencil.GetView());
    context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    opaqueVSs[VertexShaderOpaque::Default]->Bind(context);
    opaquePSs[PixelShaderOpaque::Default]->Bind(context);
    context->GSSetShader(nullptr, nullptr, 0);

    sampler.Bind(context, 0);

    D3D11_VIEWPORT viewport = {};
    viewport.Width = static_cast<float>(game->Display->ClientWidth);
    viewport.Height = static_cast<float>(game->Display->ClientHeight);
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.MinDepth = 0;
    viewport.MaxDepth = 1.0f;

    context->RSSetViewports(1, &viewport);

    vsTransformCBuf.Data.ViewPosition = Vector4(Camera::Main->Transform.GetPosition().x, Camera::Main->Transform.GetPosition().y, Camera::Main->Transform.GetPosition().z, 1.0f);

    for (auto m : meshes)
    {
        if (!m->IsEnabled())
            continue;

        //Bind ps resources
        psMaterialCBuf.Data = m->GetMaterial()->GetMaterialData();
        psMaterialCBuf.Apply(context);
        psMaterialCBuf.Bind(context, 1);

        if (m->GetMaterial()->DiffuseTexture != nullptr) m->GetMaterial()->DiffuseTexture->Bind(context, 0);
        if (m->GetMaterial()->NormalMapTexture != nullptr) m->GetMaterial()->NormalMapTexture->Bind(context, 1);
        if (m->GetMaterial()->SpecularMapTexture != nullptr) m->GetMaterial()->SpecularMapTexture->Bind(context, 2);

        //Set vs constant buffer
        vsTransformCBuf.Data.WorldMatrix = DirectX::XMMatrixTranspose(m->Transform.GetTransformMatrix());
        vsTransformCBuf.Data.WorldViewProjMatrix = DirectX::XMMatrixTranspose(m->Transform.GetTransformMatrix() * Camera::Main->GetViewProjectionMatrix());
        vsTransformCBuf.Apply(context);
        vsTransformCBuf.Bind(context, 0);

        //Draw mesh
        m->Draw();
    }
}

void RenderSystem::LightPass()
{
    ID3D11DeviceContext* context = game->Gfx.GetContext();

    context->ClearState();

    gBuffer.rtDiffuse.BindAsTexture(context, 0);
    gBuffer.rtEmissive.BindAsTexture(context, 1);
    gBuffer.rtNormals.BindAsTexture(context, 2);
    gBuffer.rtSpecular.BindAsTexture(context, 3);
    gBuffer.rtWorldPos.BindAsTexture(context, 4);

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
    game->Gfx.BindRenderTarget(depthStencil.GetView());

    TransformCbuf tBuf;
    tBuf.ViewPosition = Vector4(Camera::Main->Transform.GetPosition().x, Camera::Main->Transform.GetPosition().y, Camera::Main->Transform.GetPosition().z, 1.0f);

    for (auto light : lights)
    {
        if (!light->IsEnabled())
            continue;

        LightType lightType = light->GetLightType();
        LightData lightData = light->GetLightData();

        XMMATRIX tm = light->Transform.GetWorldTranslationMatrix();
        XMMATRIX rm = light->Transform.GetWorldRotationMatrix();
        XMMATRIX sm;

        if (lightType == LightType::Point)
        {
            sm = XMMatrixScaling(lightData.Params.x, lightData.Params.x, lightData.Params.x);
        }
        else if (lightType == LightType::Spot)
        {
            float angleFactor = lightData.Params.x * (lightData.Direction.w / (XM_PI * 0.25f));
            sm = XMMatrixScaling(angleFactor, angleFactor, lightData.Params.x);
            Vector3 rotRad = light->Transform.GetRotation() * kDeg2Rad;
            rm = XMMatrixRotationRollPitchYaw(rotRad.x + XM_PI, rotRad.y, rotRad.z);
        }
        else
        {
            sm = XMMatrixScaling(1.0f, 1.0f, 1.0f);
        }

        XMMATRIX transformMat = sm * rm * tm;

        tBuf.WorldMatrix = DirectX::XMMatrixTranspose(transformMat);
        tBuf.WorldViewProjMatrix = DirectX::XMMatrixTranspose(transformMat * Camera::Main->GetViewProjectionMatrix());

        //Set vs constant buffers
        vsTransformCBuf.Data = tBuf;
        vsTransformCBuf.Apply(context);
        vsTransformCBuf.Bind(context, 0);

        //Set ps constant buffers
        psTransformCBuf.Data = tBuf;
        psTransformCBuf.Apply(context);
        psTransformCBuf.Bind(context, 0);

        psLightCBuf.Data = lightData;
        psLightCBuf.Apply(context);
        psLightCBuf.Bind(context, 1);
        
        bsLight.Bind(context);

        if (lightType == LightType::Directional || lightType == LightType::Ambient)
        {
            rStateCullBack.Bind(context);
            dsStateLightLessEqual.Bind(context);

            lightVSs[VertexShaderLight::ScreenQuad]->Bind(context);
            lightPSs[light->GetPixelShaderLightType()]->Bind(context);
            context->GSSetShader(nullptr, nullptr, 0);

            context->IASetInputLayout(nullptr);
            context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

            context->IASetIndexBuffer(nullptr, DXGI_FORMAT_R32_UINT, 0);
            context->Draw(4, 0);
        }
        else
        {
            rStateCullFront.Bind(context);
            dsStateLightGreater.Bind(context);
            //rStateCullBack.Bind(context);
            //dsStateLightLessEqual.Bind(context);

            lightVSs[VertexShaderLight::Mesh]->Bind(context);
            lightPSs[light->GetPixelShaderLightType()]->Bind(context);

            light->GetMesh()->Draw(context);
        }
    }
}

void RenderSystem::TransparentPass()
{
    ID3D11DeviceContext* context = game->Gfx.GetContext();

    rStateCullBack.Bind(context);
    dsStateWriteOff.Bind(context);

    for (auto p : particleSystems)
    {
        p->Bind();
        p->Draw();
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

void RenderSystem::RegisterParticleSystem(ParticleSystem* particleSystem)
{
    particleSystems.push_back(particleSystem);
}

GBuffer* RenderSystem::GetGBuffer()
{
    return &gBuffer;
}
