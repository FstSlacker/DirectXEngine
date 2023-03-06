#include "ImGuiManager.h"

void ImGuiManager::DestroyWindow(int ind)
{
	delete windows[ind];
	windows.erase(windows.begin() + ind);
}

ImGuiManager::ImGuiManager()
{
	this->isEnabled = true;
}

void ImGuiManager::Initialize(ID3D11Device* device, ID3D11DeviceContext* context, void* hWnd)
{
	//Set ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(device, context);
	ImGui::StyleColorsDark();
}

void ImGuiManager::AddWindow(ImGuiBaseWindow* window)
{
	windows.push_back(window);
}

size_t ImGuiManager::GetWindowsCount() const
{
	return windows.size();
}

void ImGuiManager::Draw()
{
	if (!isEnabled)
		return;

	//Start new frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	//Create windows
	for (int i = 0; i < windows.size(); i++)
	{
		if (!windows[i]->IsOpened())
		{
			DestroyWindow(i);
			i--;
			continue;
		}
		windows[i]->Begin();
		windows[i]->Bind();
		windows[i]->End();
	}

	//Assemble and draw data
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

bool ImGuiManager::IsEnabled() const
{
	return this->isEnabled;
}

void ImGuiManager::SetEnable(bool isEnable)
{
	this->isEnabled = isEnable;
}

void ImGuiManager::DestroyResouces()
{
	for (int i = 0; i < windows.size(); i++) 
	{
		delete windows[i];
	}
	ImGui::DestroyContext();
}
