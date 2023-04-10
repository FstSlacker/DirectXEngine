#pragma once
#include <DirectXMath.h>
#include "GraphicResource.h"
#include "ConstantBuffer.h"
#include "RenderJob.h"
#include "Shaders.h"
#include "Texture.h"

class Game;
class Graphics;

struct MaterialCBuf
{
    DirectX::XMFLOAT4 Emissive; // 16
    DirectX::XMFLOAT4 Ambient; // 16
    DirectX::XMFLOAT4 Diffuse; // 16
    DirectX::XMFLOAT4 Specular; // 16

    int32_t Flags;
};

class Material : public RenderJob, public GraphicResource
{
    friend class Graphics;

public:
    Color EmissiveColor;
    Color AmbientColor;
    Color DiffuseColor;
    Color SpecularColor;
    float SpecularPower;

    Texture* DiffuseTexture;
    Texture* NormalMapTexture;
    Texture* SpecularMapTexture;

    Material(Game* game, VertexShader* vs, PixelShader* ps, UINT passInd = 1000);
    bool AttachToComponent(GameComponent& comp);
    bool RemoveFromComponent(GameComponent& comp);
    bool Initialize(ID3D11Device* device) override { return true; };

    MaterialCBuf GetMaterialData() const;

protected:
    void Bind(ID3D11DeviceContext* context) override {};

private:
    enum
    {
        USE_DIFFUSE_TEXTURE   = 1,
        USE_NORMAL_MAP        = 2,
        USE_SPECULAR_MAP      = 4
    };

    PixelShader* pixelShader;
    VertexShader* vertexShader;
};

