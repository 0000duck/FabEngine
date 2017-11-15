#pragma once

#include <DirectXComponentsPCH.h>
#include "IEntity.h"

namespace Fab
{
	enum class LightType
	{
		POINT, SPOT, DIRECTIONAL
	};

	enum LightTypeShader
	{
		POINT = 1, SPOT = 2, DIECTIONAL = 0
	};

	class Light : public IEntity
	{
	public:
		Light(LightType type);
		virtual ~Light() = 0;
		void      SetColor(XMFLOAT4 color);
		XMFLOAT4  GetColor();
		LightType GetType();

	public:
		static const XMFLOAT4 DefaultColor;

	protected:
		D3D11RenderSystem& _renderSystem;

		XMFLOAT4  _color;
		LightType _type;
	};

	typedef std::shared_ptr<Light> LightPtr;
}
