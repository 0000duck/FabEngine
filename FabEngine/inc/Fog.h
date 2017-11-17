#pragma once

#include <DirectXComponentsPCH.h>
#include "Effect.h"

namespace Fab
{
	class Fog : public IEffect
	{
	public:
		Fog(XMFLOAT4 fogColor, float fogStart, float fogRange);
		~Fog();
		void Initialise();
		void Update(float deltaTime, float totalTime);
		void Draw();

	private:
		D3D11RenderSystem& _renderSystem;

		XMFLOAT4           _fogColor;
		float              _fogStart;
		float              _fogRange;
	};
}