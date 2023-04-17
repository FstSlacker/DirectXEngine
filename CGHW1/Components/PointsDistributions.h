#pragma once
#include "../Graphics/ImGui/Source/imgui.h"
#include <SimpleMath.h>
#include <random>
#include <math.h>

enum class DistributionType
{
	Point = 0,
	Linear = 1,
	Sphere = 2,
	Cone = 3,
	Box = 4
};

class PointDistribution;
class LinearDistribution;
class SphereDistribution;
class ConeDistribution;
class BoxDistribution;

class Distribution
{
public:

	virtual DistributionType GetDistributionType() const = 0;
	virtual DirectX::SimpleMath::Vector3 GetRandomInnerPoint() = 0;
	virtual DirectX::SimpleMath::Vector3 GetRandomSurfacePoint() = 0;
	virtual void DrawGui() = 0;
};

class PointDistribution : public Distribution
{
public:
	virtual DistributionType GetDistributionType() const override
	{
		return DistributionType::Point;
	}

	virtual DirectX::SimpleMath::Vector3 GetRandomInnerPoint() override
	{
		return DirectX::SimpleMath::Vector3::Zero;
	}

	virtual DirectX::SimpleMath::Vector3 GetRandomSurfacePoint() override
	{
		return DirectX::SimpleMath::Vector3::Zero;
	}

	virtual void DrawGui() override
	{
		//...
	}
};

class LinearDistribution : public Distribution
{
public:
	DirectX::SimpleMath::Vector3 Vector = DirectX::SimpleMath::Vector3::Zero;

	virtual DistributionType GetDistributionType() const override
	{
		return DistributionType::Linear;
	}

	virtual DirectX::SimpleMath::Vector3 GetRandomInnerPoint() override
	{
		return Vector * ((float)rand() / (float)(RAND_MAX));
	}

	virtual DirectX::SimpleMath::Vector3 GetRandomSurfacePoint() override
	{
		return Vector;
	}

	virtual void DrawGui() override
	{
		ImGui::DragFloat3("Vector###LineVector", (float*)(&Vector));
	}
};

class SphereDistribution : public Distribution
{
public:
	float Radius = 2.0f;

	virtual DistributionType GetDistributionType() const override
	{
		return DistributionType::Sphere;
	}

	virtual DirectX::SimpleMath::Vector3 GetRandomInnerPoint() override
	{
		float r = ((float)rand() / (float)(RAND_MAX)) * Radius;
		float a1 = ((float)rand() / (float)(RAND_MAX)) * DirectX::XM_2PI;
		float a2 = ((float)rand() / (float)(RAND_MAX)) * DirectX::XM_2PI;

		DirectX::SimpleMath::Vector3 p = {
			cos(a1) * sin(a2) * r,
			cos(a2) * r,
			sin(a1) * sin(a2) * r
		};

		return p;
	}

	virtual DirectX::SimpleMath::Vector3 GetRandomSurfacePoint() override
	{
		float r = Radius;
		float a1 = ((float)rand() / (float)(RAND_MAX)) * DirectX::XM_2PI;
		float a2 = ((float)rand() / (float)(RAND_MAX)) * DirectX::XM_2PI;

		DirectX::SimpleMath::Vector3 p = {
			cos(a1) * sin(a2) * r,
			cos(a2) * r,
			sin(a1) * sin(a2) * r
		};

		return p;
	}

	virtual void DrawGui() override
	{
		ImGui::DragFloat("Radius###SphereRadius", &Radius);
	}
};

class ConeDistribution : public Distribution
{
public:
	float Height = 2.0f;
	float ConeAngle = 30.0f;

	virtual DistributionType GetDistributionType() const override
	{
		return DistributionType::Cone;
	}

	virtual DirectX::SimpleMath::Vector3 GetRandomInnerPoint() override
	{
		float h = Height * ((float)rand() / (float)(RAND_MAX));
		float r = tan(ConeAngle * DirectX::XM_PI / 180.0f) * h * ((float)rand() / (float)(RAND_MAX));
		float a = ((float)rand() / (float)(RAND_MAX)) * DirectX::XM_2PI;

		DirectX::SimpleMath::Vector3 p = {
			cos(a) * r,
			h,
			sin(a) * r
		};

		return p;
	}

	virtual DirectX::SimpleMath::Vector3 GetRandomSurfacePoint() override
	{
		float h = Height * ((float)rand() / (float)(RAND_MAX));
		float r = tan(ConeAngle * DirectX::XM_PI / 180.0f) * h;
		float a = ((float)rand() / (float)(RAND_MAX)) * DirectX::XM_2PI;

		DirectX::SimpleMath::Vector3 p = {
			cos(a) * r,
			h,
			sin(a) * r
		};

		return p;
	}

	virtual void DrawGui() override
	{
		ImGui::DragFloat("Height###ConeHeight", &Height);
		ImGui::DragFloat("ConeAngle", &ConeAngle);
	}
};

class BoxDistribution : public Distribution
{
public:
	DirectX::SimpleMath::Vector3 Size = DirectX::SimpleMath::Vector3::One * 2.0f;

	virtual DistributionType GetDistributionType() const override
	{
		return DistributionType::Box;
	}

	virtual DirectX::SimpleMath::Vector3 GetRandomInnerPoint() override
	{
		DirectX::SimpleMath::Vector3 p = {
			Size.x * ((float)rand() / (float)(RAND_MAX)),
			Size.y * ((float)rand() / (float)(RAND_MAX)),
			Size.z * ((float)rand() / (float)(RAND_MAX))
		};

		p *= 2.0f;
		p -= Size;

		return p;
	}

	virtual DirectX::SimpleMath::Vector3 GetRandomSurfacePoint() override
	{
		float s[3] = { 
			((float)rand() / (float)(RAND_MAX)), 
			((float)rand() / (float)(RAND_MAX)), 
			((float)rand() / (float)(RAND_MAX)) 
		};

		s[rand() % 3] = 1.0f;

		DirectX::SimpleMath::Vector3 p = {
			Size.x * s[0],
			Size.y * s[1],
			Size.z * s[2]
		};

		p *= 2.0f;
		p -= Size;

		return p;
	}

	virtual void DrawGui() override
	{
		ImGui::DragFloat3("Size###BoxDistSize", (float*)(&Size));
	}
};