#include "DebugGizmos.h"
#include "Game.h"

void DebugGizmos::DrawAxis(GameComponent* comp)
{
	Transform3D& t = comp->Transform;

	DebugDraw::DrawRay(
		primitiveBatch.get(),
		t.GetPosition(),
		t.GetForward(),
		true,
		DirectX::Colors::Blue
	);
	DebugDraw::DrawRay(
		primitiveBatch.get(),
		t.GetPosition(),
		t.GetUp(),
		true,
		DirectX::Colors::Green
	);
	DebugDraw::DrawRay(
		primitiveBatch.get(),
		t.GetPosition(),
		t.GetRight(),
		true,
		DirectX::Colors::Red
	);
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
	this->CollidersColor = Color(DirectX::Colors::LightGreen);
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

		if (ShowAxis)
		{
			DrawAxis(game->Components[i]);
		}
		if (ShowColliders)
		{
			DrawCollider(game->Components[i]);
		}
	}
	primitiveBatch->End();
}
