#pragma once
#include <SimpleMath.h>
#include <d3d11.h>

constexpr float kDeg2Rad = 3.141592653589793238463f / 180.0f;

using namespace DirectX::SimpleMath;
using namespace DirectX;

struct Transform3D 
{
private:
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;

	Vector3 forward;
	Vector3 right;
	Vector3 up;
	Vector3 backward;
	Vector3 left;
	Vector3 down;

	XMMATRIX translationMat;
	XMMATRIX rotationMat;
	XMMATRIX scaleMat;

	XMMATRIX worldMat;

	void UpdateRotation(Vector3 eulerAngles);
	void UpdateWorldMatrix();

public:

	Transform3D(Vector3 position, Vector3 rotation, Vector3 scale);
	Transform3D();

	void SetPosition(Vector3 position);
	void SetRotation(Vector3 eulerAngles);
	void SetScale(Vector3 scale);

	void AddRotation(Vector3 addEulerAngles);
	void AddPosition(Vector3 addPosition);

	const Vector3& GetPosition() const;
	const Vector3& GetRotation() const;
	const Vector3& GetScale() const;

	const Vector3& GetForward() const;
	const Vector3& GetRight() const;
	const Vector3& GetUp() const;
	const Vector3& GetBackward() const;
	const Vector3& GetLeft() const;
	const Vector3& GetDown() const;

	const DirectX::XMMATRIX& GetTranslationMatrix() const;
	const DirectX::XMMATRIX& GetRotationMatrix() const;
	const DirectX::XMMATRIX& GetScaleMatrix() const;
	const DirectX::XMMATRIX& GetTransformMatrix() const;
	const DirectX::XMMATRIX& GetTransposedTransformMatrix() const;

};