#pragma once
#include <SimpleMath.h>
#include <d3d11.h>

constexpr float kDeg2Rad = 3.141592653589793238463f / 180.0f;

using namespace DirectX::SimpleMath;

struct Transform3D 
{
private:
	DirectX::XMMATRIX translationMat;
	DirectX::XMMATRIX rotationMat;
	DirectX::XMMATRIX scaleMat;

public:

	Transform3D(Vector3 position, Vector3 rotation, Vector3 scale)
	{
		SetPosition(position);
		SetRotation(rotation);
		SetScale(scale);
	}

	Transform3D() 
	{
		translationMat = DirectX::XMMatrixIdentity();
		rotationMat = DirectX::XMMatrixIdentity();
		scaleMat = DirectX::XMMatrixIdentity();
	}

	void SetPosition(Vector3 position) 
	{
		translationMat = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	}

	Vector3 GetPosition()
	{
		DirectX::XMVECTOR pos;
		DirectX::XMVECTOR rot;
		DirectX::XMVECTOR scale;
		
		DirectX::XMMatrixDecompose(&scale, &rot, &pos, translationMat);

		return Vector3(DirectX::XMVectorGetX(pos), DirectX::XMVectorGetY(pos), DirectX::XMVectorGetZ(pos));
	}

	void SetRotation(Vector3 eulerAngles) 
	{
		eulerAngles *= kDeg2Rad;

		rotationMat = DirectX::XMMatrixRotationRollPitchYaw(eulerAngles.x, eulerAngles.y, eulerAngles.z);
	}

	Vector3 GetRotation()
	{
		return Vector3::Zero;
	}

	void SetScale(Vector3 scale)
	{
		scaleMat = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	}

	Vector3 GetScale()
	{
		DirectX::XMVECTOR pos;
		DirectX::XMVECTOR rot;
		DirectX::XMVECTOR scale;

		DirectX::XMMatrixDecompose(&scale, &rot, &pos, scaleMat);

		return Vector3(DirectX::XMVectorGetX(scale), DirectX::XMVectorGetY(scale), DirectX::XMVectorGetZ(scale));
	}

	DirectX::XMMATRIX GetTransposedTransformMatrix()
	{
		DirectX::XMMATRIX transformMat = scaleMat * rotationMat * translationMat;
		transformMat = DirectX::XMMatrixTranspose(transformMat);

		return transformMat;
	}

};