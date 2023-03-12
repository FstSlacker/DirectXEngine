#include "Transform.h"

void Transform3D::UpdateDirVectors(Vector3 eulerRadAngles)
{
	XMMATRIX rotationMat = XMMatrixRotationRollPitchYaw(eulerRadAngles.x, eulerRadAngles.y, eulerRadAngles.z);

	forward = XMVector3TransformCoord(Vector3(0.0f, 0.0f, 1.0f), rotationMat);
	right = XMVector3TransformCoord(Vector3::Right, rotationMat);
	up = XMVector3TransformCoord(Vector3::Up, rotationMat);
	backward = forward * -1.0f;
	left = right * -1.0f;
	down = up * -1.0f;
}

void Transform3D::UpdateLocalTransform()
{
	Vector3 rotDeg = localRotation * kDeg2Rad;

	XMMATRIX tm = XMMatrixTranslation(localPosition.x, localPosition.y, localPosition.z);
	XMMATRIX rm = XMMatrixRotationRollPitchYaw(rotDeg.x, rotDeg.y, rotDeg.z);
	XMMATRIX sm = XMMatrixScaling(localScale.x, localScale.y, localScale.z);

	localMat = sm * rm * tm;

	XMMATRIX transformMat = GetTransformMatrix();

	XMVECTOR p, r, s;
	DirectX::XMMatrixDecompose(&s, &r, &p, transformMat);

	Vector3 newAnglesRad = Quaternion(r).ToEuler();

	this->position = p;
	this->rotation = newAnglesRad * kRad2Deg;
	this->scale = s;

	UpdateDirVectors(newAnglesRad);

	UpdateChildsTransform();
}

void Transform3D::UpdateWorldTransform()
{
	Vector3 rotDeg = rotation * kDeg2Rad;

	XMMATRIX tm = XMMatrixTranslation(position.x, position.y, position.z);
	XMMATRIX rm = XMMatrixRotationRollPitchYaw(rotDeg.x, rotDeg.y, rotDeg.z);
	XMMATRIX sm = XMMatrixScaling(scale.x, scale.y, scale.z);

	XMMATRIX transformMat = sm * rm * tm;

	localMat = transformMat * XMMatrixInverse(nullptr, worldMat);

	XMVECTOR p, r, s;
	DirectX::XMMatrixDecompose(&s, &r, &p, localMat);

	Vector3 newAnglesRad = Quaternion(r).ToEuler();

	this->localPosition = p;
	this->localRotation = newAnglesRad * kRad2Deg;
	this->localScale = s;

	UpdateDirVectors(newAnglesRad);

	UpdateChildsTransform();
}

void Transform3D::UpdateChildsTransform()
{
	for (int i = 0; i < childs.size(); i++)
	{
		childs[i]->worldMat = GetTransformMatrix();
		childs[i]->UpdateLocalTransform();
	}
}

int Transform3D::FindChild(Transform3D& child)
{
	for (int i = 0; i < childs.size(); i++)
	{
		if (childs[i] == &child)
		{
			return i;
		}
	}
	return -1;
}

Transform3D::Transform3D(GameComponent& comp)
{
	this->gameComponent = &comp;
	this->worldMat = XMMatrixIdentity();

	SetPosition(Vector3::Zero);
	SetRotation(Vector3::Zero);
	SetScale(Vector3::One);
}

GameComponent* Transform3D::GetGameComponent() const
{
	return this->gameComponent;
}

void Transform3D::SetParent(Transform3D* parent)
{
	if (parent == this)
		throw std::exception("Parent of transform should not equal self");

	if (this->parent != nullptr)
	{
		this->parent->RemoveChild(*this);
	}

	if (parent != nullptr)
	{
		parent->AddChild(*this);
	}
}

Transform3D* Transform3D::GetParent() const
{
	return this->parent;
}

void Transform3D::AddChild(Transform3D& child)
{
	if (&child == this)
		throw std::exception("Child of transform should not equal self");

	if (FindChild(child) != -1)
		throw std::exception("Child already exists");

	if (child.parent != nullptr)
		child.parent->RemoveChild(child);


	childs.push_back(&child);
	child.parent = this;
	child.worldMat = this->GetTransformMatrix();
	child.UpdateWorldTransform();
}

void Transform3D::RemoveChild(Transform3D& child)
{
	int ind = FindChild(child);
	if (ind != -1) 
	{
		childs.erase(childs.begin() + ind);
		child.parent = nullptr;
		child.worldMat = XMMatrixIdentity();
		child.UpdateWorldTransform();
	}
}

Transform3D* Transform3D::GetChild(int ind) const
{
	return childs[ind];
}

int Transform3D::GetChildsCount() const
{
	return childs.size();
}

void Transform3D::SetPosition(Vector3 position)
{
	this->position = position;
	//translationMat = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	UpdateWorldTransform();
}

const Vector3& Transform3D::GetPosition() const
{
	return this->position;
}

void Transform3D::SetRotation(Vector3 eulerAngles)
{
	this->rotation = eulerAngles;
	eulerAngles *= kDeg2Rad;
	//this->rotationMat = DirectX::XMMatrixRotationRollPitchYaw(eulerAngles.x, eulerAngles.y, eulerAngles.z);
	UpdateWorldTransform();
}

const Vector3& Transform3D::GetRotation() const
{
	return this->rotation;
}

void Transform3D::SetScale(Vector3 scale)
{
	this->scale = scale;
	//scaleMat = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	UpdateWorldTransform();
}

void Transform3D::SetLocalPosition(Vector3 position)
{
	this->localPosition = position;
	//translationMat = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	UpdateLocalTransform();
}

void Transform3D::SetLocalRotation(Vector3 eulerAngles)
{
	this->localRotation = eulerAngles;
	eulerAngles *= kDeg2Rad;
	//this->rotationMat = DirectX::XMMatrixRotationRollPitchYaw(eulerAngles.x, eulerAngles.y, eulerAngles.z);
	UpdateLocalTransform();
}

void Transform3D::SetLocalScale(Vector3 scale)
{
	this->localScale = scale;
	//scaleMat = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	UpdateLocalTransform();
}

void Transform3D::AddRotation(Vector3 addEulerAngles)
{
	rotation += addEulerAngles;
	SetRotation(rotation);
}

void Transform3D::AddPosition(Vector3 addPosition)
{
	position += addPosition;
	SetPosition(position);
}

void Transform3D::AddLocalRotation(Vector3 addEulerAngles)
{
	localRotation += addEulerAngles;
	SetLocalRotation(localRotation);
}

void Transform3D::AddLocalPosition(Vector3 addPosition)
{
	localPosition += addPosition;
	SetLocalPosition(localPosition);
}

void Transform3D::RotateAroundAxis(Vector3 axis, float angleDeg)
{
	XMMATRIX localRot = XMMatrixRotationRollPitchYaw(
		localRotation.x * kDeg2Rad,
		localRotation.y * kDeg2Rad,
		localRotation.z * kDeg2Rad
	);

	XMMATRIX addRot = XMMatrixRotationAxis(axis, angleDeg * kDeg2Rad);

	localRot = localRot * addRot;
	
	Quaternion q = XMQuaternionRotationMatrix(localRot);

	Vector3 rotDeg = q.ToEuler() * kRad2Deg;
	SetLocalRotation(rotDeg);
}

Vector3 Transform3D::TransformPoint(Vector3 point) const
{
	XMMATRIX transMat = XMMatrixTranslation(point.x, point.y, point.z) * GetTransformMatrix();
	return Vector3(transMat.r[3]);
}

const Vector3& Transform3D::GetScale() const
{
	return this->scale;
}

const Vector3& Transform3D::GetLocalPosition() const
{
	return this->localPosition;
}

const Vector3& Transform3D::GetLocalRotation() const
{
	return this->localRotation;
}

const Vector3& Transform3D::GetLocalScale() const
{
	return this->localScale;
}

const Vector3& Transform3D::GetForward() const
{
	return this->forward;
}

const Vector3& Transform3D::GetRight() const
{
	return this->right;
}

const Vector3& Transform3D::GetUp() const
{
	return this->up;
}

const Vector3& Transform3D::GetBackward() const
{
	return this->backward;
}

const Vector3& Transform3D::GetLeft() const
{
	return this->left;
}

const Vector3& Transform3D::GetDown() const
{
	return this->down;
}

const DirectX::XMMATRIX& Transform3D::GetWorldRotationMatrix() const
{
	Vector3 rotRad = this->rotation * kDeg2Rad;
	return XMMatrixRotationRollPitchYaw(rotRad.x, rotRad.y, rotRad.z);
}

const DirectX::XMMATRIX& Transform3D::GetTransformMatrix() const
{
	return this->localMat * this->worldMat;
}

const DirectX::XMMATRIX& Transform3D::GetTransposedTransformMatrix() const
{
	return DirectX::XMMatrixTranspose(GetTransformMatrix());
}