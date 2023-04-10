#pragma once
#include "../Graphics/Bindable.h"
#include "../Graphics/VertexBuffer.h"
#include "../Graphics/IndexBuffer.h"
#include "../Graphics/Vertex.h"
#include <vector>

class Mesh
{
private:
	std::shared_ptr<VertexBuffer<Vertex>> vertexBuffer;
	std::shared_ptr<IndexBuffer> indexBuffer;

	std::vector<Vertex> vertices;
	std::vector<int> indices;

public:
	Mesh();

	void SetVertices(std::vector<Vertex> verts);
	void SetIndices(std::vector<int> inds);

	virtual bool Initialize(ID3D11Device* device);
	virtual void Draw(ID3D11DeviceContext* context);
	virtual void DestroyResources();

	static std::shared_ptr<Mesh> CreateSphereMesh(UINT tslX = 16U, UINT tslY = 16U, float radius = 1.0f);
	static std::shared_ptr<Mesh> CreateCubeMesh(float size = 1.0f);
	static std::shared_ptr<Mesh> CreatePlaneMesh(float size = 1.0f);
	static std::shared_ptr<Mesh> CreateConeMesh(UINT tsl = 16U, float radius = 1.0f, float height = 2.0f);
	static std::shared_ptr<Mesh> CreateSimpleConeMesh(UINT tsl = 16U, float radius = 1.0f, float height = 2.0f);
};

