#pragma once

#include <DirectXComponentsPCH.h>
#include "Light.h"

namespace Fab
{
	class PointLight : public Light
	{
	public:
		PointLight(LightType type = LightType::POINT);
		~PointLight();
		virtual void    Initialise() override;
		virtual void    Draw() override;
		virtual void    Update(float deltaTime, float totalTime) override;
		void            SetPosition(XMFLOAT3 position);
		void            SetRadius(float radius);
		const XMFLOAT3& GetPosition() const;
		const float     GetRadius() const;

	public:
		static const XMFLOAT3 DefaultPosition;
		static const float DefaultRadius;

	protected:
		XMFLOAT3   _position;
		float      _radius;
	};
}
