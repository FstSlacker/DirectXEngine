#include "ResourceManager.h"

std::wstring ResourceManager::kShaderFolder = L"./Shaders/";

ResourceManager::ResourceManager()
{
	//...
}

bool ResourceManager::InitializeResources(ID3D11Device* device)
{
	for (int i = 0; i < resources.size(); i++)
	{
		for (auto& r : resources[i])
		{
			if (!r.second->Initialize(device))
			{
				return false;
			}
		}
	}

	return true;
}

int ResourceManager::Find(int arrayInd, std::string& name) const
{
	for (int i = 0; i < resources[arrayInd].size(); i++)
	{
		if (resources[arrayInd][i].first == name)
		{
			return i;
		}
	}
	return -1;
}
