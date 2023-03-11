#pragma once
#include <DirectXMath.h>
#include "ConstantBuffer.h"
#include "Bindable.h"
#include "Texture.h"
#include "Shaders.h"

class Graphics;

class Material : public Bindable
{
public:
    Color EmissiveColor;
    Color AmbientColor;
    Color DiffuseColor;
    Color SpecularColor;
    float SpecularPower;

    Texture* DiffuseTexture;

    Material(VertexShader* vs, PixelShader* ps);

    bool Initialize(ID3D11Device* device);
    virtual void Bind(ID3D11DeviceContext* context) override;
    virtual void DestroyResources() override;

private:
    struct MaterialCbuf
    {
        DirectX::XMFLOAT4 Emissive; // 16
        DirectX::XMFLOAT4 Ambient; // 16
        DirectX::XMFLOAT4 Diffuse; // 16
        DirectX::XMFLOAT4 Specular; // 16

        float SpecularPower; // 4
        bool UseTexture; // 4
    };

    PixelShader* pixelShader;
    VertexShader* vertexShader;
    MaterialCbuf parameters;
    PSConstantBuffer<MaterialCbuf> materialBuffer;
};

