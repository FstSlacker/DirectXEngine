#include "SphereComponent.h"
#include <math.h>

void SphereComponent::CreateSphereMesh(size_t tslX, size_t tslY)
{
	float xStep = DirectX::XM_PI / (tslX - 1);
	float yStep = DirectX::XM_2PI / (tslY - 1);

	for (int x = 0; x < tslX; x++)
	{
		for (int y = 0; y < tslY; y++)
		{
			float t = (float)x / (float)tslX;
			this->points.push_back(
				Vertex(
					Vector3(
						cos(y * yStep) * sin(x * xStep),
						cos(x * xStep),
						sin(y * yStep) * sin(x * xStep)
					),
					DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f),
					Vector2(
						(float)y / (float)(tslY - 1),
						(float)x / (float)(tslX - 1)
					)
				)
			);
		}
	}

	for (int x = 0; x < tslX - 1; x++)
	{
		for (int y = 0; y < tslY - 1; y++)
		{
			this->indices.push_back(x * tslX + y + 1);       // 1
			this->indices.push_back(x * tslX + y);           // 0
			this->indices.push_back((x + 1) * tslX + y);     // 4

			this->indices.push_back(x * tslX + y + 1);       // 1
			this->indices.push_back((x + 1) * tslX + y);     // 4
			this->indices.push_back((x + 1) * tslX + y + 1); // 5
		}
	}

}

SphereComponent::SphereComponent(Game* game, float radius, size_t tesselation) 
	: MeshComponent(game)
{
	CreateSphereMesh(tesselation, tesselation);
}
