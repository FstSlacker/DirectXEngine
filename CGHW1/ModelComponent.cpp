#include "ModelComponent.h"
#include "Game.h"

bool ModelComponent::LoadModel(const std::string& path)
{
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

	MeshComponent* meshComp = new MeshComponent(game);
	meshComp->Transform.SetPosition(this->Transform.GetPosition());
	this->Transform.AddChild(meshComp->Transform);

	meshComp->Name = mesh->mName.C_Str();
	meshComp->SetVertices(verts);
	meshComp->SetIndices(inds);

	meshComp->SetShaders(vs, ps);
	meshComp->SetTexture(tex);
}

void ModelComponent::ProcessMaterial(aiMaterial* material, aiTextureType texType, const aiScene* scene)
{
	Texture* texture;

	UINT textureCount = material->GetTextureCount(texType);

	if (textureCount == 0)
	{
		aiColor3D diffColor(0.0f, 0.0f, 0.0f);

		switch (texType)
		{
		case aiTextureType_DIFFUSE:
			material->Get(AI_MATKEY_COLOR_DIFFUSE, diffColor);
			if (diffColor.IsBlack())
			{
				texture = new Texture(Color(0.5f, 0.5f, 0.5f));
				return;
			}
			texture = new Texture(Color(diffColor.r, diffColor.g, diffColor.b));
			return;
		}
	}
	else
	{
		texture = new Texture(Color(Colors::Red));
	}
}

ModelComponent::ModelComponent(Game* game, std::string modelPath, std::wstring texturePath) : GameComponent(game)
{
	this->Name = "ModelComponent_" + std::to_string(game->Components.size());

	ps = new PixelShader(L"./Shaders/DefaultTexture.hlsl");
	vs = new VertexShader(L"./Shaders/DefaultTexture.hlsl");
	tex = new Texture(texturePath);

	game->Gfx.AddPixelShader(ps);
	game->Gfx.AddVertexShader(vs);
	game->Gfx.AddTexture(tex);

	LoadModel(modelPath);
}
