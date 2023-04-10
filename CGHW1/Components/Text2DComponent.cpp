#include "Text2DComponent.h"
#include "../Game.h"

Text2DComponent::Text2DComponent(Game* game) : GameComponent(game)
{
	this->Name = "Text2D_" + std::to_string(game->Components.size());
}

bool Text2DComponent::Initialize()
{
	if (!GameComponent::Initialize())
		return false;

	spriteBatch = std::make_unique<DirectX::SpriteBatch>(game->Gfx.GetContext());
	spriteFont = std::make_unique<DirectX::SpriteFont>(game->Gfx.GetDevice(), L"./Fonts/comic_sans_ms.spritefont");

	return true;
}

void Text2DComponent::Draw()
{
	GameComponent::Draw();
	Vector3 scale = Transform.GetScale();
	Vector3 pos = Transform.GetPosition();

	spriteBatch->Begin();
	spriteFont->DrawString(
		spriteBatch.get(),
		Text.c_str(),
		DirectX::XMFLOAT2(pos.x, pos.y),
		DirectX::Colors::White,
		0.0f,
		DirectX::XMFLOAT2(0.0f, 0.0f),
		DirectX::XMFLOAT2(scale.x, scale.y)
	);
	spriteBatch->End();
}
