#pragma once
#include <unordered_set>
#include <SimpleMath.h>
#include <DirectXCollision.h>
#include "Delegates.h"

using namespace DirectX::SimpleMath;

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
protected:

public:
	Vector3 Size;

	AABBCollider(GameComponent* gameComp, Vector3 size);
	bool CheckIntersection(ColliderBase* otherCollider) override;
	void HandleCollision(ColliderBase* otherCollider) override;
	DirectX::BoundingBox GetWorldBoundingBox();
};
