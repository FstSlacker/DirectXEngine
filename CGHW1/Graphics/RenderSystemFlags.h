#pragma once
enum class VertexShaderOpaque
{
	Default
};

enum class PixelShaderOpaque
{
	Default
};

enum class VertexShaderLight
{
	Mesh,
	ScreenQuad
};

enum class PixelShaderLight
{
	AmbientLight,
	DirectionalLight,
	PointLight,
	SpotLight
};