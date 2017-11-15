#include <DirectXComponentsPCH.h>
#include "SpotLight.h"
#include "VectorHelper.h"

namespace Fab
{
	const float SpotLight::DefaultInnerAngle = 0.5f;
	const float SpotLight::DefaultOuterAngle = 0.5f;

	SpotLight::SpotLight()
		: PointLight(LightType::SPOT)
		, _right(Vector3Helper::Right)
		, _direction(Vector3Helper::Forward)
		, _up(Vector3Helper::Up)
		, _innerAngle(DefaultInnerAngle)
		, _outerAngle(DefaultOuterAngle)
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
		/*FrameConstantBuffer* pFrameConstantBufferUpdate = _renderSystem.GetFrameConstantBufferUpdate();
		ID3D11Buffer** pFrameConstantBuffer             = _renderSystem.GetFrameConstantBuffer();
		ID3D11DeviceContext** pContext                  = _renderSystem.GetPImmediateContext();

		pFrameConstantBufferUpdate->LightColor      = _color;
		pFrameConstantBufferUpdate->LightPosition   = XMFLOAT4(_position.x, _position.y, _position.z, 1.0f);
		pFrameConstantBufferUpdate->LightDirection  = XMFLOAT4(_direction.x, _direction.y, _direction.z, 1.0f);
		pFrameConstantBufferUpdate->LightRadius     = XMFLOAT4(_radius, 0.0f, 0.0f, 0.0f);
		pFrameConstantBufferUpdate->LightInnerAngle = XMFLOAT4(_innerAngle, 0.0f, 0.0f, 0.0f);
		pFrameConstantBufferUpdate->LightOuterAngle = XMFLOAT4(_outerAngle, 0.0f, 0.0f, 0.0f);
		pFrameConstantBufferUpdate->LightType       = XMFLOAT4(2.0f, 0.0f, 0.0f, 0.0f);

		(*pContext)->UpdateSubresource(*pFrameConstantBuffer, 0, nullptr, &pFrameConstantBufferUpdate, 0, 0);*/
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