#pragma once

#include <DirectXComponentsPCH.h>
#include "PointLight.h"

namespace Fab
{
	class SpotLight : public PointLight
	{
	public:
		SpotLight();
		~SpotLight();
		void           Initialise() override;
		void           Draw() override;
		void           Update(float deltaTime, float totalTime) override;

		const XMFLOAT3& GetDirection() const;
		const XMFLOAT3& GetUp() const;
		const XMFLOAT3& GetRight() const;

		float           GetInnerAngle();
		void            SetInnerAngle(float value);

		float           GetOuterAngle();
		void            SetOuterAngle(float value);

		void            ApplyRotation(CXMMATRIX transform);
		void            ApplyRotation(const XMFLOAT4X4& transform);

	public:
		static const float DefaultInnerAngle;
		static const float DefaultOuterAngle;

	protected:
		XMFLOAT3 _direction;
		XMFLOAT3 _up;
		XMFLOAT3 _right;
		float    _innerAngle;
		float    _outerAngle;
	};
}
