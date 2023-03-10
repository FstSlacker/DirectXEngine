#pragma once
#include <DirectXMath.h>
#include "ConstantBuffer.h"
#include "Bindable.h"

class Material : public Bindable
{
public:
    struct MaterialCbuf
    {
        DirectX::XMFLOAT4 Emissive; // 16
        DirectX::XMFLOAT4 Ambient; // 16
        DirectX::XMFLOAT4 Diffuse; // 16
        DirectX::XMFLOAT4 Specular; // 16

        float SpecularPower; // 4
        bool UseTexture; // 4
    };

    MaterialCbuf Parameters;

    HRESULT Initialize(ID3D11Device* device);
    virtual void Bind(ID3D11DeviceContext* context) override;
    virtual void DestroyResources() override;

private:
    PSConstantBuffer<MaterialCbuf> materialBuffer;

};

