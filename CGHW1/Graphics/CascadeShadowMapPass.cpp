#include "CascadeShadowMapPass.h"
#include "Graphics.h"
#include "../Components/Light.h"
#include "../Components/Camera.h"
#include "Gizmos/DebugGizmos.h"
#include <SimpleMath.h>
#include <DirectXCollision.h>
#include <limits>
#include <vector>
#include <math.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;

void CascadeShadowMapPass::BindDirectionalLight(DirectionalLightComponent& dirLight)
{
	this->directionalLight = &dirLight;
}

void CascadeShadowMapPass::BindMainCamera(Camera& cam)
{
	if(mainCamera == nullptr)
		this->mainCamera = &cam;
}

void CascadeShadowMapPass::CalculateOrthoProjections()
{
	/*const std::vector<Camera*> renderCameras = directionalLight->GetRenderCameras();

	UINT count = renderCameras.size();

	for (int i = 0; i < renderCameras.size(); i++)
	{
		renderCameras[i]->Transform.SetPosition(mainCamera->Transform.GetPosition());
		renderCameras[i]->Transform.SetRotation(directionalLight->Transform.GetRotation());
	}

	Quaternion lightOrientation = Quaternion::CreateFromRotationMatrix(directionalLight->Transform.GetWorldRotationMatrix());
	
	std::vector<Vector3> corners = GetCorners(mainCamera->GetViewMatrix(), mainCamera->GetProjectionMatrix());

	Vector3 dir0 = corners[1] - corners[0];
	Vector3 dir1 = corners[3] - corners[2];
	Vector3 dir2 = corners[5] - corners[4];
	Vector3 dir3 = corners[7] - corners[6];

	float dists[4] = { 0.0f, 25.0f, 90.0f, 200.0f };

	for (UINT i = 0; i < count; i++)
	{
		float n = dists[i] / mainCamera->GetFar();
		float f = dists[i + 1] / mainCamera->GetFar();

		Vector3 points[8];

		points[0] = corners[0] + dir0 * n;
		points[1] = corners[2] + dir1 * n;
		points[2] = corners[4] + dir2 * n;
		points[3] = corners[6] + dir3 * n;

		points[4] = corners[0] + dir0 * f;
		points[5] = corners[2] + dir1 * f;
		points[6] = corners[4] + dir2 * f;
		points[7] = corners[6] + dir3 * f;

		Vector3 pointsCenter = Vector3::Zero;

		for (auto& p : points)
		{
			pointsCenter += p;
		}

		pointsCenter /= 8.0f;

		XMMATRIX lightV = XMMatrixLookAtLH(pointsCenter, directionalLight->Transform.GetForward() + pointsCenter, directionalLight->Transform.GetUp());

		Vector3 minCorner(std::numeric_limits<float>::max());
		Vector3 maxCorner(std::numeric_limits<float>::min());

		for (UINT j = 0; j < 8; j++)
		{
			points[j] = XMVector3Transform(points[j], lightV);

			minCorner.x = minCorner.x > points[j].x ? points[j].x : minCorner.x;
			minCorner.y = minCorner.y > points[j].y ? points[j].y : minCorner.y;
			minCorner.z = minCorner.z > points[j].z ? points[j].z : minCorner.z;

			maxCorner.x = maxCorner.x < points[j].x ? points[j].x : maxCorner.x;
			maxCorner.y = maxCorner.y < points[j].y ? points[j].y : maxCorner.y;
			maxCorner.z = maxCorner.z < points[j].z ? points[j].z : maxCorner.z;
		}

		Vector2 frustrumBounds = Vector2(mainCamera->GetAspectRatio() * (maxCorner.y - minCorner.y), maxCorner.y - minCorner.y);

		Logs::Log(std::to_string(frustrumBounds.x) + " " + std::to_string(frustrumBounds.y), false);

		Vector3 fWorldUnitsPerTexel = Vector3(frustrumBounds.x / 1200.0f, frustrumBounds.y / 700.0f, 1.0f);

		Vector3 extends = (maxCorner - minCorner) * 0.5f;
		Vector3 center = (maxCorner + minCorner) * 0.5f;

		center.x /= fWorldUnitsPerTexel.x;
		center.x = floor(center.x);
		center.x *= fWorldUnitsPerTexel.x;

		center.y /= fWorldUnitsPerTexel.y;
		center.y = floor(center.y);
		center.y *= fWorldUnitsPerTexel.y;

		renderCameras[i]->Transform.SetRotation(lightOrientation.ToEuler() * kRad2Deg);
		renderCameras[i]->Transform.SetPosition(center - extends.z * directionalLight->Transform.GetForward());
		renderCameras[i]->SetOrthographicSize(extends.y * 2.0f);
		renderCameras[i]->SetNear(0.0f);
		renderCameras[i]->SetFar(extends.z * 2.0f);
		renderCameras[i]->SetAspectRatio(mainCamera->GetAspectRatio());

		renderCameras[i]->UpdateViewMatrix();
		renderCameras[i]->UpdateProjectionMatrix();
	}*/
}

std::vector<Vector3> CascadeShadowMapPass::GetCorners(const XMMATRIX& view, const XMMATRIX& proj)
{
	auto viewProj = view * proj;
	auto invert = XMMatrixInverse(nullptr, viewProj);

	std::vector<Vector3> corners;
	corners.reserve(8);
	for (int x = 0; x < 2; x++)
	{
		for (int y = 0; y < 2; y++)
		{
			for (int z = 0; z < 2; z++)
			{
				Vector4 corn = XMVector4Transform(
					Vector4(
						2.0f * x - 1.0f,
						2.0f * y - 1.0f,
						z,
						1.0f
					),
					invert
				);
				Vector4 res = corn / corn.w;
				corners.push_back(Vector3(res.x, res.y, res.z));

			}
		}
	}

	return corners;
}

void CascadeShadowMapPass::Execute(Graphics& gfx)
{
	//Camera* mainCamera = Camera::Main;

	//int cascadeMapsCount = directionalLight->GetRenderCameras().size();

	//CalculateOrthoProjections();

	//for (int i = 0; i < cascadeMapsCount; i++)
	//{
	//	Camera::Main = directionalLight->GetRenderCameras()[i];

	//	this->renderTarget = directionalLight->GetRenderTargets()[i].get();
	//	this->depthStencil = directionalLight->GetDepthBuffer();

	//	RenderQueuePass::Execute(gfx);

	//}

	//Camera::Main = mainCamera;
}
