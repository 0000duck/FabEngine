#include <DirectXComponentsPCH.h>
#include "DirectionalLight.h"
#include "VectorHelper.h"

namespace Fab
{
	DirectionalLight::DirectionalLight()
		: Light(LightType::DIRECTIONAL)
		, _right(Vector3Helper::Right)
		, _direction(Vector3Helper::Backward)
		, _up(Vector3Helper::Up)
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
		FrameConstantBuffer* pFrameConstantBufferUpdate = _renderSystem.GetFrameConstantBufferUpdate();
		ID3D11Buffer** pFrameConstantBuffer             = _renderSystem.GetFrameConstantBuffer();
		ID3D11DeviceContext** pContext                  = _renderSystem.GetPImmediateContext();

		pFrameConstantBufferUpdate->DirectionalColor     = _color;
		pFrameConstantBufferUpdate->DirectionalDirection = XMFLOAT4(_direction.x, _direction.y, _direction.z, 1.0f);

		(*pContext)->UpdateSubresource(*pFrameConstantBuffer, 0, nullptr, &pFrameConstantBufferUpdate, 0, 0);
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