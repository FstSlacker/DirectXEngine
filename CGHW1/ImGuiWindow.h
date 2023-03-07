#pragma once
#include "Graphics/ImGui/imgui.h"
#include <string>

class ImGuiBaseWindow
{
protected:
	std::string name;
	bool isOpened;

public:
	ImGuiBaseWindow(std::string name);
	virtual bool IsOpened() const;
	void Begin();
	virtual void Bind() {}
	void End();
};

