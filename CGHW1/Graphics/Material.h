#pragma once
#include <DirectXMath.h>
#include "ConstantBuffer.h"
#include "Bindable.h"
#include "Shaders.h"
#include "Texture.h"

class Game;
class Graphics;

class Material : public Bindable
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

    Material(Game* game, VertexShader* vs, PixelShader* ps);

    virtual void Bind(ID3D11DeviceContext* context) override;
    virtual void DestroyResources() override;

private:
    enum
    {
        USE_DIFFUSE_TEXTURE   = 1,
        USE_NORMAL_MAP        = 2,
        USE_SPECULAR_MAP      = 4
    };

    struct MaterialCbuf
    {
        DirectX::XMFLOAT4 Emissive; // 16
        DirectX::XMFLOAT4 Ambient; // 16
        DirectX::XMFLOAT4 Diffuse; // 16
        DirectX::XMFLOAT4 Specular; // 16

        int32_t Flags;
    };

    bool Initialize(ID3D11Device* device);

    PixelShader* pixelShader;
    VertexShader* vertexShader;
    MaterialCbuf parameters;
    PSConstantBuffer<MaterialCbuf> materialBuffer;
};

