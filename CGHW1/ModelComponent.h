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

	bool LoadModel(const std::string& path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	void ProcessMesh(aiMesh* mesh, const aiScene* scene);
	void ProcessMaterial(aiMaterial* material, aiTextureType texType, const aiScene* scene);

public:
	ModelComponent(Game* game, std::string modelPath, std::wstring texturePath);

};

