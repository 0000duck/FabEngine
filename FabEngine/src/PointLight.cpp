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
	}

	void PointLight::Update(float deltaTime, float totalTime)
	{
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