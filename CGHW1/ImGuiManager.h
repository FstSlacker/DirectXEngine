#pragma once
#include <d3d11.h>
#include "Graphics/ImGui/imgui.h"
#include "Graphics/ImGui/imgui_impl_win32.h"
#include "Graphics/ImGui/imgui_impl_dx11.h"
#include "ImGuiWindow.h"
#include <vector>

class ImGuiManager
{
private:
	std::vector<ImGuiBaseWindow*> windows;

	void DestroyWindow(int ind);

public:
	void Initialize(ID3D11Device* device, ID3D11DeviceContext* context, void* hWnd);
	void AddWindow(ImGuiBaseWindow* window);
	size_t GetWindowsCount() const;
	void Draw();
	void DestroyResouces();
};