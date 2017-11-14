#include <DirectXComponentsPCH.h>
#include "Light.h"

namespace Fab
{
	const XMFLOAT4 Light::DefaultColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	Light::Light(LightType type)
		: _renderSystem(D3D11RenderSystem::GetRenderSystem())
		, _type(type)
		, _color(DefaultColor)
	{
	}

	Light::~Light()
	{
	}

	void Light::SetColor(XMFLOAT4 color)
	{
		_color = color;
	}

	XMFLOAT4 Light::GetColor()
	{
		return _color;
	}

	LightType Light::GetType()
	{
		return _type;
	}
}