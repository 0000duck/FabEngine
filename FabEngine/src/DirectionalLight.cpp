#include <DirectXComponentsPCH.h>
#include "DirectionalLight.h"

namespace Fab
{
	DirectionalLight::DirectionalLight()
		: Light(LightType::DIRECTIONAL)
		, _right(XMFLOAT3(1.0f, 0.0f, 0.0f))
		, _direction(XMFLOAT3(0.0f, -0.5f, 1.5f))
		, _up(XMFLOAT3(0.0f, 1.0f, 0.0f))
	{
	}

	DirectionalLight::~DirectionalLight()
	{
	}

	void DirectionalLight::Initialise()
	{
	}

	void DirectionalLight::Draw()
	{
	}

	void DirectionalLight::Update(float deltaTime, float totalTime)
	{
	}

	const XMFLOAT3& DirectionalLight::GetDirection() const
	{
		return _direction;
	}

	const XMFLOAT3& DirectionalLight::GetUp() const
	{
		return _up;
	}

	const XMFLOAT3& DirectionalLight::GetRight() const
	{
		return _right;
	}

	void DirectionalLight::ApplyRotation(CXMMATRIX transform)
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

	void DirectionalLight::ApplyRotation(const XMFLOAT4X4& transform)
	{
		XMMATRIX transformMatrix = XMLoadFloat4x4(&transform);
		ApplyRotation(transformMatrix);
	}
}