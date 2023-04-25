#include "Mesh.h"
#include "Transform.h"
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

void Mesh::RecalculateTangents()
{
	int vertexCount = vertices.size();
	Vector3* tan1 = new Vector3[vertexCount * 2];
	Vector3* tan2 = tan1 + vertexCount;
	ZeroMemory(tan1, vertexCount * sizeof(Vector3) * 2);

	for (int a = 0; a < indices.size() / 3; a+=3)
	{
		int i1 = indices[a];
		int i2 = indices[a + 1];
		int i3 = indices[a + 2];

		const Vector3& v1 = vertices[i1].Position;
		const Vector3& v2 = vertices[i2].Position;
		const Vector3& v3 = vertices[i3].Position;

		const Vector2& w1 = vertices[i1].UV;
		const Vector2& w2 = vertices[i2].UV;
		const Vector2& w3 = vertices[i3].UV;

		float x1 = v2.x - v1.x;
		float x2 = v3.x - v1.x;
		float y1 = v2.y - v1.y;
		float y2 = v3.y - v1.y;
		float z1 = v2.z - v1.z;
		float z2 = v3.z - v1.z;

		float s1 = w2.x - w1.x;
		float s2 = w3.x - w1.x;
		float t1 = w2.y - w1.y;
		float t2 = w3.y - w1.y;

		float r = 1.0F / (s1 * t2 - s2 * t1);
		Vector3 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
			(t2 * z1 - t1 * z2) * r);
		Vector3 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
			(s1 * z2 - s2 * z1) * r);

		tan1[i1] += sdir;
		tan1[i2] += sdir;
		tan1[i3] += sdir;

		tan2[i1] += tdir;
		tan2[i2] += tdir;
		tan2[i3] += tdir;

		//triangle++;
	}

	for (int a = 0; a < vertexCount; a++)
	{
		const Vector3& n = vertices[a].Normal;
		const Vector3& t = tan1[a];

		// Gram-Schmidt orthogonalize
		vertices[a].Tangent = (t - n * n.Dot(t));
		vertices[a].Tangent.Normalize();

		Logs::Log(std::to_string(t.x) + ", "
			+ std::to_string(t.y) + ", "
			+ std::to_string(t.z), false);

		// Calculate handedness
		float w = (n.Cross(t).Dot(tan2[a]) < 0.0f) ? -1.0f : 1.0f;

		vertices[a].Bitangent = n.Cross(vertices[a].Tangent) * w;
		vertices[a].Bitangent.Normalize();
	}

	delete[] tan1;
}

std::shared_ptr<Mesh> Mesh::CreateSphereMesh(UINT tslX, UINT tslY, float radius)
{
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();

	std::vector<Vertex> vertices;
	std::vector<int> indices;

	float xStep = DirectX::XM_PI / (tslX - 1);
	float yStep = DirectX::XM_2PI / (tslY - 1);

	for (int x = 0; x < tslX; x++)
	{
		for (int y = 0; y < tslY; y++)
		{
			float t = (float)x / (float)tslX;

			Vector3 pos = Vector3(
				cos(y * yStep) * sin(x * xStep),
				cos(x * xStep),
				sin(y * yStep) * sin(x * xStep)
			);
			Vector2 uv = Vector2(
				(float)y / (float)(tslY - 1),
				(float)x / (float)(tslX - 1)
			);
			Vector3 norm = pos;
			norm.Normalize();
			Color color = Color(DirectX::Colors::White);
			vertices.push_back(Vertex(pos, color, uv, norm));
		}
	}

	for (int x = 0; x < tslX - 1; x++)
	{
		for (int y = 0; y < tslY - 1; y++)
		{
			indices.push_back((x + 1) * tslX + y);     // 4
			indices.push_back(x * tslX + y);           // 0
			indices.push_back(x * tslX + y + 1);       // 1

			indices.push_back((x + 1) * tslX + y + 1); // 5
			indices.push_back((x + 1) * tslX + y);     // 4
			indices.push_back(x * tslX + y + 1);       // 1
		}
	}

	mesh->SetVertices(vertices);
	mesh->SetIndices(indices);
	return mesh;
}

std::shared_ptr<Mesh> Mesh::CreateCubeMesh(float size)
{
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();

	std::vector<Vertex> vertices;
	std::vector<int> indices;

	Color color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	Vector3 dirs[] = { Transform3D::Forward, Transform3D::Right, Transform3D::Up };

	for (int dir = 0; dir < 3; dir++)
	{
		Vector3 direction = dirs[dir];
		Vector3 forward = direction * 0.5f * size;
		Vector3 right = dirs[(dir + 1) % 3] * 0.5f * size;
		Vector3 up = dirs[(dir + 2) % 3] * 0.5f * size;

		int vertsCount;

		vertsCount = vertices.size();

		vertices.push_back(Vertex(forward + right + up, color, Vector2(1.0f, 1.0f), direction));
		vertices.push_back(Vertex(forward - right - up, color, Vector2(0.0f, 0.0f), direction));
		vertices.push_back(Vertex(forward + right - up, color, Vector2(1.0f, 0.0f), direction));
		vertices.push_back(Vertex(forward - right + up, color, Vector2(0.0f, 1.0f), direction));

		indices.push_back(vertsCount + 0); indices.push_back(vertsCount + 1); indices.push_back(vertsCount + 2);
		indices.push_back(vertsCount + 1); indices.push_back(vertsCount + 0); indices.push_back(vertsCount + 3);

		vertsCount = vertices.size();

		vertices.push_back(Vertex(-forward + right + up, color, Vector2(1.0f, 1.0f), -direction));
		vertices.push_back(Vertex(-forward - right - up, color, Vector2(0.0f, 0.0f), -direction));
		vertices.push_back(Vertex(-forward + right - up, color, Vector2(1.0f, 0.0f), -direction));
		vertices.push_back(Vertex(-forward - right + up, color, Vector2(0.0f, 1.0f), -direction));

		indices.push_back(vertsCount + 2); indices.push_back(vertsCount + 1); indices.push_back(vertsCount + 0);
		indices.push_back(vertsCount + 3); indices.push_back(vertsCount + 0); indices.push_back(vertsCount + 1);
	}

	mesh->SetVertices(vertices);
	mesh->SetIndices(indices);

	return mesh;
}

std::shared_ptr<Mesh> Mesh::CreatePlaneMesh(float size)
{
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();

	Color color = Color(0.0f, 0.0f, 0.0f, 1.0f);

	std::vector<Vertex> vertices = {
		Vertex(Vector3(0.5f, 0.0f, 0.5f) * size, color, Vector2(1.0f, 1.0f), Vector3::Up),
		Vertex(Vector3(-0.5f, 0.0f, -0.5f) * size, color, Vector2(0.0f, 0.0f), Vector3::Up),
		Vertex(Vector3(0.5f, 0.0f, -0.5f) * size, color, Vector2(1.0f, 0.0f), Vector3::Up),
		Vertex(Vector3(-0.5f, 0.0f, 0.5f) * size, color, Vector2(0.0f, 1.0f), Vector3::Up)
	};

	std::vector<int> indices = { 2,1,0, 3,0,1 };

	mesh->SetVertices(vertices);
	mesh->SetIndices(indices);

	return mesh;
}

std::shared_ptr<Mesh> Mesh::CreateConeMesh(UINT tsl, float radius, float height)
{
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();

	UINT tesselation = tsl;

	Color color = Color(DirectX::Colors::White);

	std::vector<Vertex> vertices;
	std::vector<int> indices;

	float side = std::sqrt(radius * radius + height * height);
	float sideRatio = radius / side;

	for (size_t i = 0; i < tesselation; i++) {
		float ratio = static_cast<float>(i) / (tesselation);
		float r = ratio * (XM_PI * 2.0f);

		float x = std::cos(r);
		float y = std::sin(r);

		Vector3 pos = Vector3(x * radius, y * radius, -height);
		Vector3 norm = Vector3(x * (1.0f - sideRatio), y * (1.0f - sideRatio), sideRatio);
		Vector2 uv = Vector2(ratio, 0.0f);

		Vertex v;
		v.Position = pos;
		v.Normal = norm;
		v.Color = color;
		v.UV = uv;

		vertices.push_back(v);
	}

	// generate triangular faces
	for (UINT i = 0; i < tesselation; i++) {

		UINT i1 = i;
		UINT i2 = (i + 1) % tesselation;
		UINT i3 = vertices.size();

		Vertex vUp;
		vUp.Position = Vector3::Zero;
		vUp.Normal = (vertices[i1].Normal + vertices[i2].Normal) * 0.5f;
		vUp.Color = color;
		vUp.UV = Vector2((vertices[i1].UV.x + vertices[i2].UV.x) * 0.5f, 1.0f);

		vertices.push_back(vUp);

		indices.push_back(i3);
		indices.push_back(i1);
		indices.push_back(i2);
	}

	mesh->SetVertices(vertices);
	mesh->SetIndices(indices);

	return mesh;
}

std::shared_ptr<Mesh> Mesh::CreateSimpleConeMesh(UINT tsl, float radius, float height)
{
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();

	UINT tesselation = tsl;

	Color color = Color(DirectX::Colors::White);

	std::vector<Vertex> vertices;
	std::vector<int> indices;

	float side = std::sqrt(radius * radius + height * height);
	float sideRatio = radius / side;

	for (size_t i = 0; i < tesselation; i++) {
		float ratio = static_cast<float>(i) / (tesselation);
		float r = ratio * (XM_PI * 2.0f);

		float x = std::cos(r);
		float y = std::sin(r);

		Vector3 pos = Vector3(x * radius, y * radius, -height);
		Vector3 norm = Vector3(x * (1.0f - sideRatio), y * (1.0f - sideRatio), sideRatio);
		Vector2 uv = Vector2(ratio, 0.0f);

		Vertex v;
		v.Position = pos;
		v.Normal = norm;
		v.Color = color;
		v.UV = uv;

		vertices.push_back(v);
	}

	Vertex vUp;
	vUp.Position = Vector3(0.0f, 0.0f, 0.0f);
	vUp.Color = color;
	vUp.Normal = Vector3(0.0f, 0.0f, 1.0f);
	vUp.UV = Vector2(0.0f, 1.0f);

	vertices.push_back(vUp);

	Vertex vDown;
	vDown.Position = Vector3(0.0f, 0.0f, -height);
	vDown.Color = color;
	vDown.Normal = Vector3(0.0f, 0.0f, -1.0f);
	vDown.UV = Vector2(0.0f, 1.0f);

	vertices.push_back(vDown);

	// generate triangular faces
	for (UINT i = 0; i < tesselation; i++) {

		UINT i1 = i;
		UINT i2 = (i + 1) % tesselation;
		UINT i3 = vertices.size() - 2;
		UINT i4 = vertices.size() - 1;

		indices.push_back(i3);
		indices.push_back(i1);
		indices.push_back(i2);

		indices.push_back(i2);
		indices.push_back(i1);
		indices.push_back(i4);
	}

	mesh->SetVertices(vertices);
	mesh->SetIndices(indices);

	return mesh;
}
