#include <DirectXComponentsPCH.h>
#include "SpotLight.h"

namespace Fab
{
	const float SpotLight::DefaultInnerAngle = 0.75f;
	const float SpotLight::DefaultOuterAngle = 0.25f;

	SpotLight::SpotLight()
		: PointLight(LightType::SPOT)
	{
	}

	SpotLight::~SpotLight()
	{
	}

	void SpotLight::Initialise()
	{
	}

	void SpotLight::Draw()
	{
	}

	void SpotLight::Update(float deltaTime, float totalTime)
	{
	}

	const XMFLOAT3& SpotLight::GetDirection() const
	{
		return _direction;
	}

	const XMFLOAT3& SpotLight::GetUp() const
	{
		return _up;
	}

	const XMFLOAT3& SpotLight::GetRight() const
	{
		return _right;
	}

	float SpotLight::GetInnerAngle()
	{
		return _innerAngle;
	}

	void SpotLight::SetInnerAngle(float value)
	{
		_innerAngle = value;
	}

	float SpotLight::GetOuterAngle()
	{
		return _outerAngle;
	}

	void SpotLight::SetOuterAngle(float value)
	{
		_outerAngle = value;
	}

	void SpotLight::ApplyRotation(CXMMATRIX transform)
	{
		XMVECTOR direction = XMLoadFloat3(&_direction);
		XMVECTOR up = XMLoadFloat3(&_up);

		direction = XMVector3TransformNormal(direction, transform);
		direction = XMVector3Normalize(direction);

		up = XMVector3TransformNormal(up, transform);
		up = XMVector3Normalize(up);

		XMVECTOR right = XMVector3Cross(direction, up);
		up = XMVector3Cross(right, direction);

		XMStoreFloat3(&_direction, direction);
		XMStoreFloat3(&_up, up);
		XMStoreFloat3(&_right, right);
	}

	void SpotLight::ApplyRotation(const XMFLOAT4X4& transform)
	{
		XMMATRIX transformMatrix = XMLoadFloat4x4(&transform);
		ApplyRotation(transformMatrix);
	}
}