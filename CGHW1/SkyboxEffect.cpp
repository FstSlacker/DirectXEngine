#include "pch.h"
#include "SkyboxEffect.h"
#include "ReadData.h"

SkyboxEffect::SkyboxEffect(ID3D11Device* device)
{
    m_vs = VertexShader(L"./Shaders/SkyboxEffect.hlsl");
    m_ps = PixelShader(L"./Shaders/SkyboxEffect.hlsl");
    
    m_vs.Initialize(device);
    m_ps.Initialize(device);
}

void SkyboxEffect::Apply(ID3D11DeviceContext* deviceContext)
{
    deviceContext->PSSetShaderResources(0, 1, m_texture.GetAddressOf());

    deviceContext->VSSetShader(m_vs.GetShader(), nullptr, 0);
    deviceContext->PSSetShader(m_ps.GetShader(), nullptr, 0);
}

void SkyboxEffect::GetVertexShaderBytecode(void const** pShaderByteCode, size_t* pByteCodeLength)
{
    assert(pShaderByteCode != nullptr && pByteCodeLength != nullptr);
    *pShaderByteCode = m_vsBlob.data();
    *pByteCodeLength = m_vsBlob.size();
}

void SkyboxEffect::SetTexture(ID3D11ShaderResourceView* value)
{
    m_texture = value;
}
