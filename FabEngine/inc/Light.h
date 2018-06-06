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

		void SetModel(ModelPtr& model);
		ModelPtr& GetModelPtr();

	public:
		static const XMFLOAT4 DefaultColor;

	protected:
		D3D11RenderSystem& _renderSystem;

		XMFLOAT4  _color;
		LightType _type;

		ModelPtr _model;
	};

	typedef std::shared_ptr<Light> LightPtr;
}
