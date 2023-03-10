#include "DebugGizmos.h"
#include "Game.h"

void DebugGizmos::DrawAxis(GameComponent* comp)
{
	Transform3D& t = comp->Transform;
	float axesSize = Vector3::Distance(t.GetPosition(), game->MainCamera->Transform.GetPosition()) * 0.1f;

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

void DebugGizmos::DrawGrid()
{
	size_t xDivs = 20;
	size_t yDivs = 20;
	float gridSize = game->MainCamera->Transform.GetPosition().Length() + 10.0f;

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

	this->ShowAxis = true;
	this->ShowColliders = true;

	this->ShowGridXZ = true;
	this->ShowGridXY = false;
	this->ShowGridYZ = false;

	this->CollidersColor = Color(DirectX::Colors::LightGreen);
	this->GridColor = Color(DirectX::Colors::LightGray);
}

void DebugGizmos::DrawLightRange(PointLightComponent* light)
{
	DirectX::BoundingSphere sphere = DirectX::BoundingSphere(
		light->Transform.GetPosition(),
		light->Range
	);

	DebugDraw::Draw(primitiveBatch.get(), sphere, DirectX::Colors::White);
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
	basicEffect->SetProjection(game->MainCamera->GetProjectionMatrix());
	basicEffect->SetView(game->MainCamera->GetViewMatrix());
	basicEffect->Apply(game->Gfx.GetContext());
	game->Gfx.GetContext()->IASetInputLayout(inputLayout.Get());

	primitiveBatch->Begin();

	for (int i = 0; i < game->Components.size(); i++)
	{
		if (typeid(*(game->Components[i])) == typeid(Camera))
			continue;

		if (!game->Components[i]->IsEnabled)
			continue;

		if (ShowAxis)
		{
			game->Gfx.SetDepthStencilEnable(false);
			DrawAxis(game->Components[i]);
		}
		if (ShowColliders)
		{
			game->Gfx.SetDepthStencilEnable(false);
			DrawCollider(game->Components[i]);
		}
	}

	//if (game->Light != nullptr)
	//{
	//	DrawLightRange(game->Light);
	//}

	if (ShowGridXZ || ShowGridXY || ShowGridYZ)
	{
		game->Gfx.SetDepthStencilEnable(true);
		DrawGrid();
	}
	primitiveBatch->End();
}
