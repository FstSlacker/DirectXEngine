#include "Mesh.h"
#include "../Logs.h"

Mesh::Mesh()
{
	vertexBuffer = std::make_shared<VertexBuffer<Vertex>>();
	indexBuffer = std::make_shared<IndexBuffer>();
}

void Mesh::SetVertices(std::vector<Vertex> verts)
{
	vertices = verts;
}

void Mesh::SetIndices(std::vector<int> inds)
{
	indices = inds;
}

bool Mesh::Initialize(ID3D11Device* device)
{
	HRESULT hr;

	hr = vertexBuffer->Initialize(device, vertices.data(), vertices.size());
	if (FAILED(hr))
	{
		Logs::LogError(hr, "Failed to initialize vertexBuffer");
		return false;
	}

	hr = indexBuffer->Initialize(device, indices.data(), indices.size());
	if (FAILED(hr))
	{
		Logs::LogError(hr, "Failed to initialize indexBuffer");
		return false;
	}

    return true;
}

void Mesh::Draw(ID3D11DeviceContext* context)
{
	vertexBuffer->Bind(context);
	indexBuffer->Bind(context);

    context->DrawIndexed(indexBuffer->BufferSize(), 0, 0);
}

void Mesh::DestroyResources()
{
    this->vertexBuffer.reset();
    this->indexBuffer.reset();
}
