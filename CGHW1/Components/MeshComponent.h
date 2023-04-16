#pragma once
#include <d3d.h>
#include <d3d11.h>
#include <directxmath.h>
#include <vector>
#include "GameComponent.h"

#include "Mesh.h"
#include "../Graphics/ConstantBuffer.h"
#include "../Graphics/Material.h"

class Game;
class GameComponent;

class MeshComponent : public GameComponent
{
protected:
	std::shared_ptr<Mesh> mesh;
	Material* material;

public:

	MeshComponent(Game* game);

	bool Initialize() override;
	void Draw() override;
	void DrawGui() override;
	void DestroyResources() override;

	void SetMesh(std::shared_ptr<Mesh> mesh);
	void SetMaterial(Material* mat);
	Material* GetMaterial() const;
};
