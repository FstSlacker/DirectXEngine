#pragma once
#include <d3d11.h>
#include <vector>
#include <GeometricPrimitive.h>
#include <Effects.h>
#include <wrl.h>
#include "Shaders.h"

class SkyboxEffect //: public DirectX::IEffect
{
public:
    explicit SkyboxEffect(ID3D11Device* device);

    void Apply(
        ID3D11DeviceContext* deviceContext); //override;

    void GetVertexShaderBytecode(
        void const** pShaderByteCode,
        size_t* pByteCodeLength); //override;

    void SetTexture(ID3D11ShaderResourceView* value);

private:
    VertexShader m_vs;
    PixelShader m_ps;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
    std::vector<uint8_t> m_vsBlob;
};
