#pragma once

#include <DirectXComponentsPCH.h>
#include "Light.h"

namespace Fab
{
	class DirectionalLight : public Light
	{
	public:
		DirectionalLight();
		~DirectionalLight();
		void            Initialise() override;
		void            Draw() override;
		void            Update(float deltaTime, float totalTime) override;

		const XMFLOAT3& GetDirection() const;
		const XMFLOAT3& GetUp() const;
		const XMFLOAT3& GetRight() const;

		void            ApplyRotation(CXMMATRIX transform);
		void            ApplyRotation(const XMFLOAT4X4& transform);

	protected:
		XMFLOAT3 _direction;
		XMFLOAT3 _up;
		XMFLOAT3 _right;
	};
}
