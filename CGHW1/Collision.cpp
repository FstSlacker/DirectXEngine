#include "Collision.h"
#include "GameComponent.h"

AABBCollider::AABBCollider(GameComponent* gameComp, Vector3 size) : ColliderBase(gameComp)
{
	this->Size = size;
}

bool AABBCollider::CheckIntersection(ColliderBase* otherCollider)
{
	ColliderBase::CheckIntersection(otherCollider);
	AABBCollider* otherAABB = dynamic_cast<AABBCollider*>(otherCollider);
	if (otherAABB != nullptr) {
		return GetWorldBoundingBox().Intersects(otherAABB->GetWorldBoundingBox());
	}

	return false;
}

void AABBCollider::HandleCollision(ColliderBase* otherCollider)
{
	ColliderBase::HandleCollision(otherCollider);
}

DirectX::BoundingBox AABBCollider::GetWorldBoundingBox()
{
	Vector3 pos = gameComponent->Transform.GetPosition();
	Vector3 scale = gameComponent->Transform.GetScale() * 0.5f;

	
	DirectX::BoundingBox bb = DirectX::BoundingBox(
		pos,
		DirectX::XMFLOAT3(Size.x * scale.x, Size.y * scale.y, Size.z * scale.z)
	);

	return bb;
}



void ColliderBase::HandleCollision(ColliderBase* otherCollider)
{
	bool isCollided = CheckIntersection(otherCollider);
	bool isContains = incomingColliders.count(otherCollider);
	if (otherCollider == nullptr)
		return;

	const CollisionArgs c = CollisionArgs{ otherCollider->gameComponent };

	if (!isContains && isCollided)
	{
		incomingColliders.insert(otherCollider);
		OnCollisionEnter.Broadcast(c);
	}
	else if (isContains && isCollided)
	{
		OnCollisionStay.Broadcast(c);
	}
	else if (isContains && !isCollided)
	{
		incomingColliders.erase(otherCollider);
		OnCollisionExit.Broadcast(c);
	}
}

ColliderBase::~ColliderBase()
{
	for(auto c : incomingColliders)
	{
		c->incomingColliders.erase(this);
	}
}

ColliderBase::ColliderBase(GameComponent* gameComp)
{
	this->gameComponent = gameComp;
}

bool ColliderBase::CheckIntersection(ColliderBase* otherCollider)
{
	return false;
}
