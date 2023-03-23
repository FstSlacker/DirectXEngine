#pragma once
#include <array>
#include <vector>
#include <string>
#include <memory>

#include "Graphics/GraphicResource.h"
#include "Graphics/Shaders.h"
#include "Graphics/Material.h"
#include "Graphics/Texture.h"

class ResourceManager
{
public:
	ResourceManager();

	template<class T>
	bool AddResource(T* resource, std::string name);

	template<class T>
	UINT GetCount() const;

	bool InitializeResources(ID3D11Device* device);

	template<class T>
	T* GetResource(UINT ind) const;

	template<class T>
	T* GetResource(std::string name) const;

	template<class T>
	const std::string& GetResourceName(UINT ind) const;

private:
	enum class ResourceType
	{
		VertexShader,
		PixelShader,
		Texture,
		Material
	};

	static std::wstring kShaderFolder;

	std::array<std::vector<std::pair<std::string, GraphicResource*>>, 4> resources;
	
	template<class T>
	int GetResourceArrayIndex() const;
	int Find(int arrayInd, std::string& name) const;
};

template<class T>
bool ResourceManager::AddResource(T* resource, std::string name)
{
	int ind = GetResourceArrayIndex<T>();
	assert(ind != -1, "T must derrive from GraphicResource");

	if (this->Find(ind, name) == -1)
	{
		resources[ind].push_back(std::pair<std::string, GraphicResource*>(name, resource));
		return true;
	}
	return false;
}

template<class T>
UINT ResourceManager::GetCount() const
{
	int arrayInd = GetResourceArrayIndex<T>();
	assert(arrayInd != -1, "T must derrive from GraphicResource");

	return resources[arrayInd].size();
}

template<class T>
T* ResourceManager::GetResource(UINT ind) const
{
	int arrayInd = GetResourceArrayIndex<T>();
	assert(arrayInd != -1, "T must derrive from GraphicResource");

	return dynamic_cast<T*>(resources[arrayInd][ind].second);
}

template<class T>
T* ResourceManager::GetResource(std::string name) const
{
	int arrayInd = GetResourceArrayIndex<T>();
	assert(arrayInd != -1, "T must derrive from GraphicResource");

	int ind = this->Find(arrayInd, name);

	if (ind != -1)
	{
		return dynamic_cast<T*>(resources[arrayInd][ind].second);
	}
	return nullptr;
}

template<class T>
inline const std::string& ResourceManager::GetResourceName(UINT ind) const
{
	int arrayInd = GetResourceArrayIndex<T>();
	assert(arrayInd != -1, "T must derrive from GraphicResource");

	return resources[arrayInd][ind].first;
}

template<class T>
int ResourceManager::GetResourceArrayIndex() const
{
	if (typeid(T) == typeid(VertexShader))
		return (int)ResourceType::VertexShader;
	else if (typeid(T) == typeid(PixelShader))
		return (int)ResourceType::PixelShader;
	else if (typeid(T) == typeid(Texture))
		return (int)ResourceType::Texture;
	else if (typeid(T) == typeid(Material))
		return (int)ResourceType::Material;

	return -1;
}


