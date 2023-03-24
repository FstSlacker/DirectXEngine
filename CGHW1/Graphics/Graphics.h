#pragma once
#include <d3d.h>
#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")

#include <iostream>
#include <vector>
#include <wrl.h>
#include <map>

#include "Material.h"
#include "DepthStencil.h"

class Graphics
{
public:
	Color BackgroundColor = Color(0.0f, 0.0f, 0.0f);

	bool Initialize(HWND hWnd, UINT width, UINT height);
	bool Resize(UINT width, UINT height);
	void DestroyResources();
	void PrepareFrame();
	void EndFrame();

	void SetDepthStencilEnable(bool isEnable);

	ID3D11DeviceContext* GetContext() const;
	ID3D11Device* GetDevice() const;

private:
	bool InitializeDirectX(HWND hWnd);
	bool InitializeResources();

	static std::wstring kShaderFolder;

	UINT displayWidth;
	UINT displayHeight;
	bool isInitialized = false;

	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderView;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
	DepthStencil depthStencil;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rastState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilStateEnabled;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilStateDisabled;
};

