#include "Text2DComponent.h"
#include "Game.h"

Text2DComponent::Text2DComponent(Game* game, Transform3D transform) : GameComponent(game, transform)
{

}

void Text2DComponent::Initialize()
{
	GameComponent::Initialize();
	spriteBatch = std::make_unique<DirectX::SpriteBatch>(game->Context);
	spriteFont = std::make_unique<DirectX::SpriteFont>(game->Device.Get(), L"./Fonts/comic_sans_ms.spritefont");
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
