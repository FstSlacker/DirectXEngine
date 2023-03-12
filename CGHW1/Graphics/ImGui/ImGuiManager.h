#pragma once
#include <d3d11.h>
#include "Source/imgui.h"
#include "Source/imgui_impl_win32.h"
#include "Source/imgui_impl_dx11.h"
#include "ImGuiWindow.h"
#include <vector>

class ImGuiManager
{
private:
	std::vector<ImGuiBaseWindow*> windows;
	bool isEnabled;

	void DestroyWindow(int ind);

public:
	ImGuiManager();
	void Initialize(ID3D11Device* device, ID3D11DeviceContext* context, void* hWnd);
	void AddWindow(ImGuiBaseWindow* window);
	size_t GetWindowsCount() const;
	void Draw();
	bool IsEnabled() const;
	void SetEnable(bool isEnable);
	void DestroyResouces();

};