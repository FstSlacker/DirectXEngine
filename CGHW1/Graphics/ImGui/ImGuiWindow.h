#pragma once
#include "Source/imgui.h"
#include <string>

class ImGuiBaseWindow
{
protected:
	std::string name;
	bool isOpened;

public:
	ImGuiBaseWindow(std::string name);
	virtual bool IsOpened() const;
	virtual void Begin();
	virtual void Bind() {}
	virtual void End();
};

