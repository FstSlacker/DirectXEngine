#include "Transform.h"

void Transform3D::UpdateDirVectors()
{
	forward = XMVector3TransformCoord(Vector3(0.0f, 0.0f, 1.0f), rotationMat);
	right = XMVector3TransformCoord(Vector3::Right, rotationMat);
	up = XMVector3TransformCoord(Vector3::Up, rotationMat);
	backward = forward * -1.0f;
	left = right * -1.0f;
	down = up * -1.0f;
}

void Transform3D::UpdateWorldMatrix()
{
	worldMat = scaleMat * rotationMat * translationMat;
}

void Transform3D::UpdateChildsTransform()
{

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
}

void Transform3D::RemoveChild(Transform3D& child)
{
	int ind = FindChild(child);
	if (ind != -1) 
	{
		childs.erase(childs.begin() + ind);
		child.parent = nullptr;
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
	translationMat = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	UpdateWorldMatrix();
}

const Vector3& Transform3D::GetPosition() const
{
	return this->position;
}

void Transform3D::SetRotation(Vector3 eulerAngles)
{
	this->rotation = eulerAngles;
	eulerAngles *= kDeg2Rad;
	this->rotationMat = DirectX::XMMatrixRotationRollPitchYaw(eulerAngles.x, eulerAngles.y, eulerAngles.z);
	UpdateDirVectors();
	UpdateWorldMatrix();
}

const Vector3& Transform3D::GetRotation() const
{
	return this->rotation;
}

void Transform3D::SetScale(Vector3 scale)
{
	this->scale = scale;
	scaleMat = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	UpdateWorldMatrix();
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
	addEulerAngles *= kDeg2Rad;
	XMMATRIX localRot = DirectX::XMMatrixRotationRollPitchYaw(addEulerAngles.x, addEulerAngles.y, addEulerAngles.z);
	this->rotationMat = localRot * this->rotationMat;
	UpdateDirVectors();
	UpdateWorldMatrix();

	XMVECTOR p, r, s;
	DirectX::XMMatrixDecompose(&s, &r, &p, this->worldMat);
	Quaternion q(r);
	this->rotation = q.ToEuler();
}

const Vector3& Transform3D::GetScale() const
{
	return this->scale;
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

const DirectX::XMMATRIX& Transform3D::GetTranslationMatrix() const
{
	return this->translationMat;
}

const DirectX::XMMATRIX& Transform3D::GetRotationMatrix() const
{
	return this->rotationMat;
}

const DirectX::XMMATRIX& Transform3D::GetScaleMatrix() const
{
	return this->scaleMat;
}

const DirectX::XMMATRIX& Transform3D::GetTransformMatrix() const
{
	if (parent != nullptr)
	{
		return this->worldMat * parent->GetTransformMatrix();
	}
	else 
	{
		return this->worldMat;
	}
}

const DirectX::XMMATRIX& Transform3D::GetTransposedTransformMatrix() const
{
	return DirectX::XMMatrixTranspose(GetTransformMatrix());
}