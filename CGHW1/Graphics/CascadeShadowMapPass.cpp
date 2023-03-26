#include "CascadeShadowMapPass.h"
#include "Graphics.h"
#include "../Components/Light.h"
#include "../Components/Camera.h"
#include "Gizmos/DebugGizmos.h"
#include <SimpleMath.h>
#include <DirectXCollision.h>
#include <limits>
#include <vector>

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
	const std::vector<Camera*> renderCameras = directionalLight->GetRenderCameras();

	UINT count = renderCameras.size();

	for (int i = 0; i < renderCameras.size(); i++)
	{
		renderCameras[i]->Transform.SetPosition(mainCamera->Transform.GetPosition());
		renderCameras[i]->Transform.SetRotation(directionalLight->Transform.GetRotation());
	}

	BoundingFrustum cameraFrustrum = BoundingFrustum(mainCamera->GetProjectionMatrix());
	cameraFrustrum.Origin = mainCamera->Transform.GetPosition();
	cameraFrustrum.Orientation = Quaternion::CreateFromRotationMatrix(mainCamera->Transform.GetWorldRotationMatrix());

	Quaternion lightOrientation = Quaternion::CreateFromRotationMatrix(directionalLight->Transform.GetWorldRotationMatrix());

	XMMATRIX lightCameraV = renderCameras[0]->GetViewMatrix();
	XMMATRIX mainCameraV = mainCamera->GetViewMatrix();
	
	Vector3 corners[8];
	cameraFrustrum.GetCorners(corners);

	Vector3 dir0 = corners[4] - corners[0];
	Vector3 dir1 = corners[5] - corners[1];
	Vector3 dir2 = corners[6] - corners[2];
	Vector3 dir3 = corners[7] - corners[3];

	dir0.Normalize();
	dir1.Normalize();
	dir2.Normalize();
	dir3.Normalize();

	float dists[4] = { 0.0f, 10.0f, 50.0f, 100.0f };

	for (UINT i = 0; i < count; i++)
	{
		float n = dists[i];
		float f = dists[i + 1];

		Vector3 points[8];
		points[0] = corners[0] + dir0 * n;
		points[1] = corners[1] + dir1 * n;
		points[2] = corners[2] + dir2 * n;
		points[3] = corners[3] + dir3 * n;

		points[4] = corners[0] + dir0 * f;
		points[5] = corners[1] + dir1 * f;
		points[6] = corners[2] + dir2 * f;
		points[7] = corners[3] + dir3 * f;

		Vector3 minCorner(std::numeric_limits<float>::max());
		Vector3 maxCorner(std::numeric_limits<float>::min());

		for (UINT j = 0; j < 8; j++)
		{
			points[j] = XMVector3Transform(points[j], lightCameraV);

			minCorner.x = minCorner.x > points[j].x ? points[j].x : minCorner.x;
			minCorner.y = minCorner.y > points[j].y ? points[j].y : minCorner.y;
			minCorner.z = minCorner.z > points[j].z ? points[j].z : minCorner.z;

			maxCorner.x = maxCorner.x < points[j].x ? points[j].x : maxCorner.x;
			maxCorner.y = maxCorner.y < points[j].y ? points[j].y : maxCorner.y;
			maxCorner.z = maxCorner.z < points[j].z ? points[j].z : maxCorner.z;
		}

		Vector3 extends = (maxCorner - minCorner) * 0.5f;
		Vector3 center = minCorner + extends;

		center = XMVector3Transform(center, XMMatrixInverse(nullptr, lightCameraV));

		renderCameras[i]->Transform.SetPosition(center + -extends.z * directionalLight->Transform.GetForward());
		renderCameras[i]->Transform.SetRotation(lightOrientation.ToEuler() * kRad2Deg);
		renderCameras[i]->SetOrthographicSize(extends.y * 2.0f);
		renderCameras[i]->SetNear(0.01f);
		renderCameras[i]->SetFar(extends.z * 2.0f);

		//renderCameras[i]->DrawGizmos();

		//gizmos.DrawOrientedBox(center, extends, lightOrientation);
	}
}

void CascadeShadowMapPass::Execute(Graphics& gfx)
{
	Camera* mainCamera = Camera::Main;

	int cascadeMapsCount = directionalLight->GetRenderCameras().size();

	CalculateOrthoProjections();

	for (int i = 0; i < cascadeMapsCount; i++)
	{
		Camera::Main = directionalLight->GetRenderCameras()[i];

		this->renderTarget = directionalLight->GetRenderTargets()[i].get();
		this->depthStencil = directionalLight->GetDepthBuffer();

		RenderQueuePass::Execute(gfx);

	}

	Camera::Main = mainCamera;
}
