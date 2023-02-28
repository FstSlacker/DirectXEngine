#include "PlanetComponent.h"
#include "Game.h"

PlanetComponent::PlanetComponent(Game* game, Vector3 position, PlanetInfo info, Texture* tex)
	: SphereComponent(game, Transform3D(position, Vector3::Zero, Vector3(info.Diameter, info.Diameter, info.Diameter)))
{
	SetTexture(tex);
	this->Info = info;
	this->Name = info.Name;
}
