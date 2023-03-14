#include "CubeComponent.h"
#include "../Game.h"

CubeComponent::CubeComponent(Game* game) : MeshComponent(game)
{
	this->Name = "Cube_" + std::to_string(game->Components.size());

	Color color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	Vector3 dirs[] = { Transform3D::Forward, Transform3D::Right, Transform3D::Up };

	for (int dir = 0; dir < 3; dir++)
	{
		Vector3 direction = dirs[dir];
		Vector3 forward = direction * 0.5f;
		Vector3 right = dirs[(dir + 1) % 3] * 0.5f;
		Vector3 up = dirs[(dir + 2) % 3] * 0.5f;

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
}
