#pragma once
#include "GameComponent.h"
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <string>

class Text2DComponent : public GameComponent
{
public:
	std::wstring Text;

	Text2DComponent(Game* game, Transform3D transform);
	void Initialize() override;
	void Draw() override;

private:
	std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
	std::unique_ptr<DirectX::SpriteFont> spriteFont;
};

