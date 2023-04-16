#include "DebugGizmos.h"
#include "../../Game.h"

void DebugGizmos::DrawAxis(GameComponent* comp)
{
	Transform3D& t = comp->Transform;
	float axesSize = Vector3::Distance(t.GetPosition(), Camera::Main->Transform.GetPosition()) * 0.1f;

	DebugDraw::DrawRay(
		primitiveBatch.get(),
		t.GetPosition(),
		t.GetForward() * axesSize,
		false,
		DirectX::Colors::Blue
	);
	DebugDraw::DrawRay(
		primitiveBatch.get(),
		t.GetPosition(),
		t.GetUp() * axesSize,
		false,
		DirectX::Colors::Green
	);
	DebugDraw::DrawRay(
		primitiveBatch.get(),
		t.GetPosition(),
		t.GetRight() * axesSize,
		false,
		DirectX::Colors::Red
	);
}

void DebugGizmos::DrawRing(Vector3 origin, Vector3 axis1, Vector3 axis2, Color color)
{
	DebugDraw::DrawRing(primitiveBatch.get(), origin, axis1, axis2, color);
}

void DebugGizmos::DrawQuad(Vector3 p1, Vector3 p2, Vector3 p3, Vector3 p4, Color color)
{
	DebugDraw::DrawQuad(primitiveBatch.get(), p1, p2, p3, p4, color);
}

void DebugGizmos::DrawRay(Vector3 origin, Vector3 direction, Color color)
{
	DebugDraw::DrawRay(primitiveBatch.get(), origin, direction, false, color);
}

void DebugGizmos::DrawSphere(Vector3 origin, float radius, Color color)
{
	DirectX::BoundingSphere sphere = DirectX::BoundingSphere(origin, radius);
	DebugDraw::Draw(primitiveBatch.get(), sphere, color);
}

void DebugGizmos::DrawFrustrum(Vector3 origin, Quaternion orientation, XMMATRIX projection, Color color)
{
	DirectX::BoundingFrustum frustrum = BoundingFrustum(projection);
	frustrum.Origin = origin;
	frustrum.Orientation = orientation;
	DebugDraw::Draw(primitiveBatch.get(), frustrum, color);
}

void DebugGizmos::DrawOrientedBox(Vector3 center, Vector3 extents, Quaternion orientation, Color color)
{
	BoundingOrientedBox box = BoundingOrientedBox(center, extents, orientation);
	DebugDraw::Draw(primitiveBatch.get(), box, color);
}

void DebugGizmos::DrawGrid()
{
	size_t xDivs = 20;
	size_t yDivs = 20;
	float gridSize = Camera::Main->Transform.GetPosition().Length() + 10.0f;

	if(ShowGridXZ)
		DebugDraw::DrawGrid(primitiveBatch.get(), Vector3::Right * gridSize, Vector3::Forward * gridSize, Vector3::Zero, xDivs, yDivs, GridColor);
	if(ShowGridXY)
		DebugDraw::DrawGrid(primitiveBatch.get(), Vector3::Right * gridSize, Vector3::Up * gridSize, Vector3::Zero, xDivs, yDivs, GridColor);
	if(ShowGridYZ)
		DebugDraw::DrawGrid(primitiveBatch.get(), Vector3::Up * gridSize, Vector3::Forward * gridSize, Vector3::Zero, xDivs, yDivs, GridColor);
}

void DebugGizmos::Initialize()
{
	primitiveBatch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>(game->Gfx.GetContext());
	basicEffect = std::make_unique<DirectX::DX11::BasicEffect>(game->Gfx.GetDevice());
	basicEffect->SetVertexColorEnabled(true);

	CreateInputLayoutFromEffect<DirectX::DX11::VertexPositionColor>(game->Gfx.GetDevice(), basicEffect.get(), inputLayout.ReleaseAndGetAddressOf());
}

DebugGizmos::DebugGizmos(Game* game)
{
	this->game = game;

	this->ShowObjectsIcons = true;
	this->ShowObjectsGizmos = false;

	this->ShowGridXZ = true;
	this->ShowGridXY = false;
	this->ShowGridYZ = false;

	this->CollidersColor = Color(DirectX::Colors::LightGreen);
	this->GridColor = Color(DirectX::Colors::LightGray);
}

void DebugGizmos::DrawCollider(GameComponent* comp)
{
	ColliderBase* colliderBase = comp->Collider;

	if (colliderBase == nullptr)
		return;

	if (typeid(*colliderBase) == typeid(SphereCollider))
	{
		SphereCollider* collider = dynamic_cast<SphereCollider*>(colliderBase);
		DebugDraw::Draw(primitiveBatch.get(), collider->GetWorldBoundingSphere(), CollidersColor);
	}
	else if (typeid(*colliderBase) == typeid(AABBCollider))
	{
		AABBCollider* collider = dynamic_cast<AABBCollider*>(colliderBase);
		DebugDraw::Draw(primitiveBatch.get(), collider->GetWorldBoundingBox(), CollidersColor);
	}
}

void DebugGizmos::Draw()
{
	game->Gfx.GetContext()->GSSetShader(nullptr, nullptr, 0);
	game->Gfx.GetContext()->PSSetShader(nullptr, nullptr, 0);
	game->Gfx.GetContext()->VSSetShader(nullptr, nullptr, 0);

	basicEffect->SetProjection(Camera::Main->GetProjectionMatrix());
	basicEffect->SetView(Camera::Main->GetViewMatrix());
	basicEffect->Apply(game->Gfx.GetContext());
	game->Gfx.GetContext()->IASetInputLayout(inputLayout.Get());

	primitiveBatch->Begin();

	for (int i = 0; i < game->Components.size(); i++)
	{
		if (!game->Components[i]->IsEnabled())
			continue;

		if (ShowObjectsGizmos)
		{
			game->Gfx.SetDepthStencilEnable(false);
			game->Components[i]->DrawGizmos();
		}
		if (ShowObjectsIcons)
		{
			Vector3 right = Camera::Main->Transform.GetRight();
			Vector3 up = Camera::Main->Transform.GetUp();
			float iconScale = (Camera::Main->Transform.GetPosition() - game->Components[i]->Transform.GetPosition()).Length() * 0.03f;
			
			game->Gfx.SetDepthStencilEnable(false);
			game->Components[i]->DrawGizmosIcon(right, up, iconScale);
		}
	}

	if (ShowGridXZ || ShowGridXY || ShowGridYZ)
	{
		game->Gfx.SetDepthStencilEnable(true);
		DrawGrid();
	}
	primitiveBatch->End();
}
