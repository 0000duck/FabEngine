#include <DirectXComponentsPCH.h>
#include "PointLight.h"

namespace Fab
{
	const XMFLOAT3 PointLight::DefaultPosition = XMFLOAT3(-2.0f, 3.0f, -2.0f);
	const float PointLight::DefaultRadius      = 1.0f;

	PointLight::PointLight(LightType type)
		: Light(type)
		, _position(DefaultPosition)
		, _radius(_radius)
	{
	}

	PointLight::~PointLight()
	{
	}

	void PointLight::Initialise()
	{
	}

	void PointLight::Draw()
	{
		FrameConstantBuffer* pFrameConstantBufferUpdate = _renderSystem.GetFrameConstantBufferUpdate();
		ID3D11Buffer** pFrameConstantBuffer             = _renderSystem.GetFrameConstantBuffer();
		ID3D11DeviceContext** pContext                  = _renderSystem.GetPImmediateContext();

		pFrameConstantBufferUpdate->LightColor    = _color;
		pFrameConstantBufferUpdate->LightPosition = XMFLOAT4(_position.x, _position.y, _position.z, 1.0f);
		pFrameConstantBufferUpdate->LightRadius   = XMFLOAT4(16.0f, 0.0f, 0.0f, 0.0f);
		pFrameConstantBufferUpdate->LightType     = XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);

		(*pContext)->UpdateSubresource(*pFrameConstantBuffer, 0, nullptr, &pFrameConstantBufferUpdate, 0, 0);
	}

	void PointLight::Update(float deltaTime, float totalTime)
	{
		Keyboard& keyboard = static_cast<Keyboard&>(Application::GetApplication().GetComponent(ComponentType::KEYBOARD));
		Mouse& mouse = static_cast<Mouse&>(Application::GetApplication().GetComponent(ComponentType::MOUSE));

		if (keyboard.IsKeyPressed(KeyName::ARROW_UP))
		{
			_position.z -= deltaTime * 4.0f;
		}

		if (keyboard.IsKeyPressed(KeyName::ARROW_DOWN))
		{
			_position.z += deltaTime * 4.0f;
		}

		if (keyboard.IsKeyPressed(KeyName::ARROW_LEFT))
		{
			_position.x += deltaTime * 4.0f;
		}

		if (keyboard.IsKeyPressed(KeyName::ARROW_RIGHT))
		{
			_position.x -= deltaTime * 4.0f;
		}

		if (keyboard.IsKeyPressed(KeyName::SPACE))
		{
			_position.y += deltaTime * 4.0f;
		}

		if (keyboard.IsKeyPressed(KeyName::CTRL))
		{
			_position.y -= deltaTime * 4.0f;
		}
	}

	void PointLight::SetPosition(XMFLOAT3 position)
	{
		_position = position;
	}

	void PointLight::SetRadius(float radius)
	{
		_radius = radius;
	}

	const XMFLOAT3& PointLight::GetPosition() const
	{
		return _position;
	}

	const float PointLight::GetRadius() const
	{
		return _radius;
	}
}