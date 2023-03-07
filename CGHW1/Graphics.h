#pragma once
#include <d3d.h>
#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")

#include <iostream>
#include <vector>
#include <wrl.h>

#include "Shaders.h"
#include "Texture.h"

class Graphics
{
public:
	Color BackgroundColor = Color(0.0f, 0.0f, 0.0f);

	bool Initialize(HWND hWnd, UINT width, UINT height);
	void DestroyResources();
	void PrepareFrame();
	void EndFrame();

	void AddVertexShader(VertexShader* vs);
	void AddPixelShader(PixelShader* ps);
	void AddTexture(Texture* tex);

	void SetDepthStencilEnable(bool isEnable);

	ID3D11DeviceContext* GetContext() const;
	ID3D11Device* GetDevice() const;

private:
	bool InitializeDirectX(HWND hWnd);
	bool InitializeResources();

	UINT displayWidth;
	UINT displayHeight;

	ID3D11DeviceContext* context;
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	ID3D11RenderTargetView* renderView;
	IDXGISwapChain* swapChain;

	std::vector<VertexShader*> vertexShaders;
	std::vector<PixelShader*> pixelShaders;
	std::vector<Texture*> textures;

	ID3D11Texture2D* backBuffer;
	ID3D11Texture2D* depthStencilBuffer;
	ID3D11DepthStencilView* depthStencilView;

	ID3D11RasterizerState* rastState;
	ID3D11DepthStencilState* depthStencilStateEnabled;
	ID3D11DepthStencilState* depthStencilStateDisabled;
};

