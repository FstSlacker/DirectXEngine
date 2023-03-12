#include "ModelComponent.h"

#include "../Game.h"
#include "../StringHelper.h"

#include "../Graphics/Vertex.h"
#include "../Graphics/Material.h"

bool ModelComponent::LoadModel(const std::string& path)
{
	this->modelPath = path;
	this->modelDirectory = FilePathHelper::GetFileDirectory(modelPath);

	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

	if (scene == nullptr)
		return false;

	ProcessNode(scene->mRootNode, scene);

}

void ModelComponent::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		ProcessMesh(mesh, scene);
	}

	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

void ModelComponent::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> verts;
	std::vector<int> inds;

	for (UINT i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vert;

		vert.Position = Vector3(
			mesh->mVertices[i].x,
			mesh->mVertices[i].y,
			mesh->mVertices[i].z
		);

		if (mesh->mTextureCoords[0])
		{
			vert.UV = Vector2(
				(float)mesh->mTextureCoords[0][i].x,
				(float)mesh->mTextureCoords[0][i].y
			);
		}

		vert.Normal = Vector3(
			mesh->mNormals[i].x,
			mesh->mNormals[i].y,
			mesh->mNormals[i].z
		);

		verts.push_back(vert);
	}

	for (UINT i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (UINT j = 0; j < face.mNumIndices; j++)
		{
			inds.push_back(face.mIndices[j]);
		}
	}

	aiMaterial* meshMaterial = scene->mMaterials[mesh->mMaterialIndex];

	std::vector<Texture*> textures;
	std::vector<Texture*> diffTextures = GetMaterialTextures(meshMaterial, aiTextureType::aiTextureType_DIFFUSE, scene);

	textures.insert(textures.end(), diffTextures.begin(), diffTextures.end());

	MeshComponent* meshComp = new MeshComponent(game);

	meshComp->Transform.SetPosition(this->Transform.GetPosition());
	this->Transform.AddChild(meshComp->Transform);

	meshComp->Name = mesh->mName.C_Str();
	meshComp->SetVertices(verts);
	meshComp->SetIndices(inds);

	Material* mat = new Material(game, game->Gfx.FindVertexShader(L"VS_Default.hlsl"), game->Gfx.FindPixelShader(L"PS_DefaultLit.hlsl"));
	mat->EmissiveColor = Color(0.0f, 0.0f, 0.0f, 0.0f);
	mat->AmbientColor = Color(0.0f, 0.0f, 0.0f);
	mat->DiffuseColor = Color(1.0f, 1.0f, 1.0f);
	mat->SpecularColor = Color(1.0f, 1.0f, 1.0f);
	mat->SpecularPower = 32.0f;

	meshComp->Material = mat;

	for (int i = 0; i < textures.size(); i++)
	{
		meshComp->Material->DiffuseTexture = textures[i];
	}
}

std::vector<Texture*> ModelComponent::GetMaterialTextures(aiMaterial* material, aiTextureType texType, const aiScene* scene)
{
	std::vector<Texture*> textures;

	UINT textureCount = material->GetTextureCount(texType);

	if (textureCount == 0)
	{
		aiColor3D diffColor(0.0f, 0.0f, 0.0f);

		switch (texType)
		{
		case aiTextureType_DIFFUSE:
			material->Get(AI_MATKEY_COLOR_DIFFUSE, diffColor);
			textures.push_back(new Texture(game, Color(diffColor.r, diffColor.g, diffColor.b)));
			return textures;
		}
	}
	else
	{
		for (UINT i = 0; i < textureCount; i++)
		{
			aiString path;
			material->GetTexture(texType, i, &path);

			std::string name = FilePathHelper::GetFileName(path.C_Str());

			TextureStorageType storeType = GetTextureStorageType(material, i, texType, scene);
			switch (storeType)
			{
			case TextureStorageType::File:
				if (!FilePathHelper::IsFileExist(path.C_Str()))
				{
					std::wstring findedFile = FilePathHelper::FindFileInParentDirectories(name, modelDirectory);
					textures.push_back(new Texture(game, findedFile));
				}
				else
				{
					textures.push_back(new Texture(game, path.C_Str()));
				}
				break;
			default:
				textures.push_back(new Texture(game, Color(1.0f, 0.0f, 0.0f)));
				break;
			}
		}

		return textures;
	}
}

TextureStorageType ModelComponent::GetTextureStorageType(aiMaterial* material, UINT ind, aiTextureType texType, const aiScene* scene)
{
	if (material->GetTextureCount(texType) == 0)
		return TextureStorageType::None;

	aiString path;
	material->GetTexture(texType, ind, &path);
	std::string texPath = path.C_Str();

	if (texPath[0] == '*')
	{
		if (scene->mTextures[0]->mHeight == 0)
		{
			return TextureStorageType::EmbeddedIndexCompressed;
		}
		else
		{
			return TextureStorageType::EmbeddedIndex;
		}
	}

	if (auto tex = scene->GetEmbeddedTexture(texPath.c_str()))
	{
		if (tex->mHeight == 0)
		{
			return TextureStorageType::EmbeddedCompressed;
		}
		else
		{
			return TextureStorageType::Embedded;
		}
	}

	if (texPath.find('.') != std::string::npos)
	{
		return TextureStorageType::File;
	}

	return TextureStorageType::None;
}

ModelComponent::ModelComponent(Game* game, std::string modelPath) : GameComponent(game)
{

	this->Name = FilePathHelper::GetFileName(modelPath);

	LoadModel(modelPath);
}
