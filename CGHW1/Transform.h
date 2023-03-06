#pragma once
#include <SimpleMath.h>
#include <d3d11.h>
#include <vector>

constexpr float kDeg2Rad = 3.141592653589793238463f / 180.0f;

using namespace DirectX::SimpleMath;
using namespace DirectX;

class GameComponent;

class Transform3D 
{
private:
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;

	Vector3 localPosition;
	Vector3 localRotation;
	Vector3 localScale;

	Vector3 forward;
	Vector3 right;
	Vector3 up;
	Vector3 backward;
	Vector3 left;
	Vector3 down;

	/*XMMATRIX translationMat;
	XMMATRIX rotationMat;
	XMMATRIX scaleMat;*/

	XMMATRIX localMat;
	XMMATRIX worldMat;

	GameComponent* gameComponent;
	Transform3D* parent;
	std::vector<Transform3D*> childs;
	Transform3D(const Transform3D& rhs);

	void UpdateDirVectors();
	void UpdateLocalTransform();
	void UpdateWorldTransform();
	void UpdateChildsTransform();
	int FindChild(Transform3D& child);

public:
	Transform3D(GameComponent& comp);

	GameComponent* GetGameComponent() const;
	void SetParent(Transform3D* parent);
	Transform3D* GetParent() const;
	void AddChild(Transform3D& child);
	void RemoveChild(Transform3D& child);
	Transform3D* GetChild(int ind) const;
	int GetChildsCount() const;

	void SetPosition(Vector3 position);
	void SetRotation(Vector3 eulerAngles);
	void SetScale(Vector3 scale);

	void SetLocalPosition(Vector3 position);
	void SetLocalRotation(Vector3 eulerAngles);
	void SetLocalScale(Vector3 scale);


	void AddRotation(Vector3 addEulerAngles);
	void AddPosition(Vector3 addPosition);

	void AddLocalRotation(Vector3 addEulerAngles);
	void AddLocalPosition(Vector3 addPosition);

	const Vector3& GetPosition() const;
	const Vector3& GetRotation() const;
	const Vector3& GetScale() const;

	const Vector3& GetLocalPosition() const;
	const Vector3& GetLocalRotation() const;
	const Vector3& GetLocalScale() const;

	const Vector3& GetForward() const;
	const Vector3& GetRight() const;
	const Vector3& GetUp() const;
	const Vector3& GetBackward() const;
	const Vector3& GetLeft() const;
	const Vector3& GetDown() const;

	const DirectX::XMMATRIX& GetWorldRotationMatrix() const;
	const DirectX::XMMATRIX& GetTransformMatrix() const;
	const DirectX::XMMATRIX& GetTransposedTransformMatrix() const;

};