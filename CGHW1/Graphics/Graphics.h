#pragma once
#include <d3d.h>
#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")

#include <iostream>
#include <vector>
#include <wrl.h>
#include <map>

#include "Material.h"
#include "DepthStencilBuffer.h"

class Graphics
{
public:
	Color BackgroundColor = Color(0.0f, 0.0f, 0.0f);

	bool Initialize(HWND hWnd, UINT width, UINT height);
	bool Resize(UINT width, UINT height);
	void DestroyResources();
	void PrepareFrame();
	void EndFrame();

	void AddVertexShader(std::wstring name);
	void AddPixelShader(std::wstring name);

	VertexShader* FindVertexShader(std::wstring name) const;
	PixelShader* FindPixelShader(std::wstring name) const;

	void AddTexture(Texture* tex);
	void AddMaterial(Material* mat);

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

	std::map<std::wstring, std::unique_ptr<VertexShader>> vertexShaders;
	std::map<std::wstring, std::unique_ptr<PixelShader>> pixelShaders;

	std::vector<Texture*> textures;
	std::vector<Material*> materials;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
	DepthStencilBuffer depthStencil;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rastState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilStateEnabled;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilStateDisabled;
};

