
//#include <windows.h>
//#include <WinUser.h>
//#include <wrl.h>
//#include <iostream>
//#include <d3d.h>
//#include <d3d11.h>
//#include <d3dcompiler.h>
//#include <directxmath.h>
//#include <chrono>
//
//
//#pragma comment(lib, "d3d11.lib")
//#pragma comment(lib, "dxgi.lib")
//#pragma comment(lib, "d3dcompiler.lib")
//#pragma comment(lib, "dxguid.lib")
//
//
//LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
//{
//	switch (umessage)
//	{
//	case WM_KEYDOWN:
//	{
//		// If a key is pressed send it to the input object so it can record that state.
//		std::cout << "Key: " << static_cast<unsigned int>(wparam) << std::endl;
//
//		if (static_cast<unsigned int>(wparam) == 27) PostQuitMessage(0);
//		return 0;
//	}
//	default:
//	{
//		return DefWindowProc(hwnd, umessage, wparam, lparam);
//	}
//	}
//}
//
//int main()
//{
//	LPCWSTR applicationName = L"My3DApp";
//	HINSTANCE hInstance = GetModuleHandle(nullptr);
//
//#pragma region Window init
//	WNDCLASSEX wc;
//
//	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
//	wc.lpfnWndProc = WndProc;
//	wc.cbClsExtra = 0;
//	wc.cbWndExtra = 0;
//	wc.hInstance = hInstance;
//	wc.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
//	wc.hIconSm = wc.hIcon;
//	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
//	wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
//	wc.lpszMenuName = nullptr;
//	wc.lpszClassName = applicationName;
//	wc.cbSize = sizeof(WNDCLASSEX);
//
//	// Register the window class.
//	RegisterClassEx(&wc);
//
//
//	auto screenWidth = 800;
//	auto screenHeight = 800;
//
//	RECT windowRect = { 0, 0, static_cast<LONG>(screenWidth), static_cast<LONG>(screenHeight) };
//	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);
//
//	auto dwStyle = WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_THICKFRAME;
//
//	auto posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
//	auto posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
//
//	HWND hWnd = CreateWindowEx(WS_EX_APPWINDOW, applicationName, applicationName,
//		dwStyle,
//		posX, posY,
//		windowRect.right - windowRect.left,
//		windowRect.bottom - windowRect.top,
//		nullptr, nullptr, hInstance, nullptr);
//
//	ShowWindow(hWnd, SW_SHOW);
//	SetForegroundWindow(hWnd);
//	SetFocus(hWnd);
//
//	ShowCursor(true);
//
//#pragma endregion Window init
//
//
//	D3D_FEATURE_LEVEL featureLevel[] = { D3D_FEATURE_LEVEL_11_1 };
//
//	DXGI_SWAP_CHAIN_DESC swapDesc = {};
//	swapDesc.BufferCount = 2;
//	swapDesc.BufferDesc.Width = screenWidth;
//	swapDesc.BufferDesc.Height = screenHeight;
//	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
//	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
//	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
//	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
//	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
//	swapDesc.OutputWindow = hWnd;
//	swapDesc.Windowed = true;
//	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
//	swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
//	swapDesc.SampleDesc.Count = 1;
//	swapDesc.SampleDesc.Quality = 0;
//
//
//	Microsoft::WRL::ComPtr<ID3D11Device> device;
//	ID3D11DeviceContext* context;
//	IDXGISwapChain* swapChain;
//
//	auto res = D3D11CreateDeviceAndSwapChain(
//		nullptr,
//		D3D_DRIVER_TYPE_HARDWARE,
//		nullptr,
//		D3D11_CREATE_DEVICE_DEBUG,
//		featureLevel,
//		1,
//		D3D11_SDK_VERSION,
//		&swapDesc,
//		&swapChain,
//		&device,
//		nullptr,
//		&context);
//
//	if (FAILED(res))
//	{
//		// Well, that was unexpected
//	}
//
//
//	ID3D11Texture2D* backTex;
//	ID3D11RenderTargetView* rtv;
//	res = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backTex);	// __uuidof(ID3D11Texture2D)
//	res = device->CreateRenderTargetView(backTex, nullptr, &rtv);
//
//	//-----------------------------------------
//
//	ID3DBlob* vertexBC = nullptr;
//	ID3DBlob* errorVertexCode = nullptr;
//	res = D3DCompileFromFile(L"./Shaders/MyVeryFirstShader.hlsl",
//		nullptr /*macros*/,
//		nullptr /*include*/,
//		"VSMain",
//		"vs_5_0",
//		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
//		0,
//		&vertexBC,
//		&errorVertexCode);
//
//	if (FAILED(res)) {
//		// If the shader failed to compile it should have written something to the error message.
//		if (errorVertexCode) {
//			char* compileErrors = (char*)(errorVertexCode->GetBufferPointer());
//
//			std::cout << compileErrors << std::endl;
//		}
//		// If there was  nothing in the error message then it simply could not find the shader file itself.
//		else
//		{
//			MessageBox(hWnd, L"MyVeryFirstShader.hlsl", L"Missing Shader File", MB_OK);
//		}
//
//		return 0;
//	}
//
//	//--------
//
//	D3D_SHADER_MACRO Shader_Macros[] = { "TEST", "1", "TCOLOR", "float4(0.0f, 1.0f, 0.0f, 1.0f)", nullptr, nullptr };
//
//	ID3DBlob* pixelBC;
//	ID3DBlob* errorPixelCode;
//	res = D3DCompileFromFile(L"./Shaders/MyVeryFirstShader.hlsl", nullptr /*macros*/, nullptr /*include*/, "PSMain", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &pixelBC, &errorPixelCode);
//
//	ID3D11VertexShader* vertexShader;
//	ID3D11PixelShader* pixelShader;
//	device->CreateVertexShader(
//		vertexBC->GetBufferPointer(),
//		vertexBC->GetBufferSize(),
//		nullptr, &vertexShader);
//
//	device->CreatePixelShader(
//		pixelBC->GetBufferPointer(),
//		pixelBC->GetBufferSize(),
//		nullptr, &pixelShader);
//
//	D3D11_INPUT_ELEMENT_DESC inputElements[] = {
//		D3D11_INPUT_ELEMENT_DESC {
//			"POSITION",
//			0,
//			DXGI_FORMAT_R32G32B32A32_FLOAT,
//			0,
//			0,
//			D3D11_INPUT_PER_VERTEX_DATA,
//			0},
//		D3D11_INPUT_ELEMENT_DESC {
//			"COLOR",
//			0,
//			DXGI_FORMAT_R32G32B32A32_FLOAT,
//			0,
//			D3D11_APPEND_ALIGNED_ELEMENT,
//			D3D11_INPUT_PER_VERTEX_DATA,
//			0}
//	};
//
//	//Формат ввода
//	ID3D11InputLayout* layout;
//	device->CreateInputLayout(
//		inputElements,
//		2,
//		vertexBC->GetBufferPointer(),
//		vertexBC->GetBufferSize(),
//		&layout);
//
//	DirectX::XMFLOAT4 points[8] = {
//		DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
//		DirectX::XMFLOAT4(-0.5f, -0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
//		DirectX::XMFLOAT4(0.5f, -0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
//		DirectX::XMFLOAT4(-0.5f, 0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
//	};
//
//
//	D3D11_BUFFER_DESC vertexBufDesc = {};
//	vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
//	vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	vertexBufDesc.CPUAccessFlags = 0;
//	vertexBufDesc.MiscFlags = 0;
//	vertexBufDesc.StructureByteStride = 0;
//	vertexBufDesc.ByteWidth = sizeof(DirectX::XMFLOAT4) * std::size(points);
//
//	D3D11_SUBRESOURCE_DATA vertexData = {};
//	vertexData.pSysMem = points;
//	vertexData.SysMemPitch = 0;
//	vertexData.SysMemSlicePitch = 0;
//
//	ID3D11Buffer* vb;
//	device->CreateBuffer(&vertexBufDesc, &vertexData, &vb);
//
//	int indeces[] = { 0,1,2, 1,0,3 };
//	D3D11_BUFFER_DESC indexBufDesc = {};
//	indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
//	indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
//	indexBufDesc.CPUAccessFlags = 0;
//	indexBufDesc.MiscFlags = 0;
//	indexBufDesc.StructureByteStride = 0;
//	indexBufDesc.ByteWidth = sizeof(int) * std::size(indeces);
//
//	D3D11_SUBRESOURCE_DATA indexData = {};
//	indexData.pSysMem = indeces;
//	indexData.SysMemPitch = 0;
//	indexData.SysMemSlicePitch = 0;
//
//	ID3D11Buffer* ib;
//	device->CreateBuffer(&indexBufDesc, &indexData, &ib);
//
//	UINT strides[] = { 32 };
//	UINT offsets[] = { 0 };
//
//
//
//	CD3D11_RASTERIZER_DESC rastDesc = {};
//	rastDesc.CullMode = D3D11_CULL_NONE;
//	rastDesc.FillMode = D3D11_FILL_SOLID;
//
//	ID3D11RasterizerState* rastState;
//	res = device->CreateRasterizerState(&rastDesc, &rastState);
//
//	context->RSSetState(rastState);
//
//
//
//
//	std::chrono::time_point<std::chrono::steady_clock> PrevTime = std::chrono::steady_clock::now();
//	float totalTime = 0;
//	unsigned int frameCount = 0;
//
//
//	MSG msg = {};
//	bool isExitRequested = false;
//	while (!isExitRequested) {
//		// Handle the windows messages.
//		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
//			TranslateMessage(&msg);
//			DispatchMessage(&msg);
//		}
//
//		// If windows signals to end the application then exit out.
//		if (msg.message == WM_QUIT) {
//			isExitRequested = true;
//		}
//
//		context->ClearState();
//
//		context->RSSetState(rastState);
//
//		D3D11_VIEWPORT viewport = {};
//		viewport.Width = static_cast<float>(screenWidth);
//		viewport.Height = static_cast<float>(screenHeight);
//		viewport.TopLeftX = 0;
//		viewport.TopLeftY = 0;
//		viewport.MinDepth = 0;
//		viewport.MaxDepth = 1.0f;
//
//
//		/*auto	curTime = std::chrono::steady_clock::now();
//		float	deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(curTime - PrevTime).count() / 1000000.0f;
//		PrevTime = curTime;
//
//		totalTime += deltaTime;
//		frameCount++;
//
//		if (totalTime > 1.0f) {
//			float fps = frameCount / totalTime;
//
//			totalTime -= 1.0f;
//
//			WCHAR text[256];
//			swprintf_s(text, TEXT("FPS: %f"), fps);
//			SetWindowText(hWnd, text);
//
//			frameCount = 0;
//		}*/
//
//		context->OMSetRenderTargets(1, &rtv, nullptr);
//
//		float color[] = { totalTime, 0.1f, 0.1f, 1.0f };
//		context->ClearRenderTargetView(rtv, color);
//
//		context->RSSetViewports(1, &viewport);
//
//		context->IASetInputLayout(layout);
//		context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//		context->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);
//		context->IASetVertexBuffers(0, 1, &vb, strides, offsets);
//		context->VSSetShader(vertexShader, nullptr, 0);
//		context->PSSetShader(pixelShader, nullptr, 0);
//		context->DrawIndexed(6, 0, 0);
//
//		context->OMSetRenderTargets(0, nullptr, nullptr);
//
//		swapChain->Present(1, /*DXGI_PRESENT_DO_NOT_WAIT*/ 0);
//	}
//
//	std::cout << "Hello World!\n";
//}











#include "Game.h"
int main()
{
	Game game = Game();
	game.Name = L"Solar System";

	PlanetComponent* sun = new PlanetComponent(
		&game,
		Vector3(0.0f, 0.0f, 0.0f),
		2.0f,
		0.0f,
		0.0f,
		Texture(L"./Textures/sun.jpg")
	);
	sun->IsStar = true;
	sun->Name = "Sun";

	PlanetComponent* mercury = new PlanetComponent(
		&game,
		Vector3(10.0f, 0.0f, 0.0f),
		0.382f,
		10.0f,
		1.607454668f,
		Texture(L"./Textures/mercury.jpg")
	);
	mercury->Name = "Mercury";

	PlanetComponent* venus = new PlanetComponent(
		&game,
		Vector3(20.0f, 0.0f, 0.0f),
		0.949f,
		20.0f,
		1.175957018f,
		Texture(L"./Textures/venus.jpg")
	);
	venus->Name = "Venus";

	PlanetComponent* earth = new PlanetComponent(
		&game,
		Vector3(30.0f, 0.0f, 0.0f),
		1.0f,
		30.0f,
		1.0f,
		Texture(L"./Textures/earth.jpg")
	);
	earth->Name = "Earth";

	/*PlanetComponent* moon = new PlanetComponent(
		&game,
		Vector3(4.0f, 0.0f, 0.0f),
		0.27f,
		Texture(L"./Textures/moon.jpg")
	);*/

	PlanetComponent* mars = new PlanetComponent(
		&game,
		Vector3(40.0f, 0.0f, 0.0f),
		0.53f,
		40.0f,
		0.808495635f,
		Texture(L"./Textures/mars.jpg")
	);
	mars->Name = "Mars";

	PlanetComponent* jupiter = new PlanetComponent(
		&game,
		Vector3(50.0f, 0.0f, 0.0f),
		11.2f,
		50.0f,
		0.438885158f,
		Texture(L"./Textures/jupiter.jpg")
	);
	jupiter->Name = "Jupiter";

	PlanetComponent* saturn = new PlanetComponent(
		&game,
		Vector3(60.0f, 0.0f, 0.0f),
		9.41f,
		60.0f,
		0.325386165f,
		Texture(L"./Textures/saturn.jpg")
	);
	saturn->Name = "Saturn";

	PlanetComponent* uranus = new PlanetComponent(
		&game,
		Vector3(70.0f, 0.0f, 0.0f),
		3.98f,
		70.0f,
		0.228676964f,
		Texture(L"./Textures/uranus.jpg")
	);
	uranus->Name = "Uranus";

	PlanetComponent* neptune = new PlanetComponent(
		&game,
		Vector3(80.0f, 0.0f, 0.0f),
		3.81f,
		80.0f,
		0.182337139f,
		Texture(L"./Textures/neptune.jpg")
	);
	neptune->Name = "Neptune";

	Text2DComponent* text = new Text2DComponent(
		&game,
		Transform3D(
			Vector3(0.0f, 218.0f, 0.0f),
			Vector3::Zero,
			Vector3::One
		)
	);

	PlanetSystemComponent* sysComp = new PlanetSystemComponent(
		&game,
		Transform3D(),
		sun,
		std::vector<PlanetComponent*>{
			mercury,
			venus,
			earth,
			mars,
			jupiter,
			saturn,
			uranus,
			neptune
		}
	);

	CameraMoveComponent* moveComp = new CameraMoveComponent(
		&game,
		Transform3D()
	);

	

	game.Components.push_back(moveComp);

	game.Components.push_back(sun);
	game.Components.push_back(mercury);
	game.Components.push_back(venus);
	game.Components.push_back(earth);
	//game.Components.push_back(moon);
	game.Components.push_back(mars);
	game.Components.push_back(jupiter);
	game.Components.push_back(saturn);
	game.Components.push_back(uranus);
	game.Components.push_back(neptune);

	game.Components.push_back(sysComp);

	game.Components.push_back(text);

	game.ImGUI.AddWindow(new ImGuiGameCompWindow(earth));
	game.ImGUI.AddWindow(new ImGuiGameCompWindow(sun));
	game.ImGUI.AddWindow(new ImGuiGameInfoWindow(&game));

	game.Run();
}