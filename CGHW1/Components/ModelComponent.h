#pragma once
#include "MeshComponent.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

class ModelComponent : public GameComponent
{
private:
	VertexShader* vs;
	PixelShader* ps;
	Texture* tex;

	std::string modelDirectory;
	std::string modelPath;

	bool LoadModel(const std::string& path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	void ProcessMesh(aiMesh* mesh, const aiScene* scene);

	Texture* GetMaterialTexture(aiMaterial* material, aiTextureType texType, const aiScene* scene);
	TextureStorageType GetTextureStorageType(aiMaterial* material, UINT ind, aiTextureType texType, const aiScene* scene);

public:
	ModelComponent(Game* game, std::string modelPath);

};

