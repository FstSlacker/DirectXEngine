#include "ImGuiWindow.h"

ImGuiBaseWindow::ImGuiBaseWindow(std::string name)
{
	this->name = name + "###" + std::to_string(reinterpret_cast<intptr_t>(this));
	this->isOpened = true;
}

bool ImGuiBaseWindow::IsOpened() const
{
	return isOpened;
}

void ImGuiBaseWindow::Begin()
{
	ImGui::Begin(name.c_str(), &isOpened);
}

void ImGuiBaseWindow::End()
{
	ImGui::End();
}

