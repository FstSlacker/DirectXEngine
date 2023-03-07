#pragma once
#include <unordered_set>
#include <SimpleMath.h>
#include <DirectXCollision.h>
#include "Delegates.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;

class GameComponent;

struct CollisionArgs
{
public:
	GameComponent* CollidedComponent;
};

class ColliderBase
{
protected:
	GameComponent* gameComponent;
	std::unordered_set<ColliderBase*> incomingColliders;

	~ColliderBase();

public:
	MulticastDelegate<const CollisionArgs&> OnCollisionEnter;
	MulticastDelegate<const CollisionArgs&> OnCollisionStay;
	MulticastDelegate<const CollisionArgs&> OnCollisionExit;

	ColliderBase(GameComponent* gameComp);
	virtual bool CheckIntersection(ColliderBase* otherCollider);
	virtual void HandleCollision(ColliderBase* otherCollider);
};

class AABBCollider : public ColliderBase
{
public:
	Vector3 Size;
	Vector3 Offsets;

	AABBCollider(GameComponent* gameComp, Vector3 size = Vector3::One, Vector3 offsets = Vector3::Zero);
	bool CheckIntersection(ColliderBase* otherCollider) override;
	void HandleCollision(ColliderBase* otherCollider) override;

	BoundingBox GetWorldBoundingBox() const;
};

class SphereCollider : public ColliderBase
{
public:
	float Radius;
	Vector3 Offsets;

	SphereCollider(GameComponent* gameComp, float radius = 1.0f, Vector3 offsets = Vector3::Zero);
	virtual bool CheckIntersection(ColliderBase* otherCollider) override;
	virtual void HandleCollision(ColliderBase* otherCollider) override;

	BoundingSphere GetWorldBoundingSphere() const;
};
