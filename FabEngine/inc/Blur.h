#pragma once

#include <DirectXComponentsPCH.h>
#include "Effect.h"

namespace Fab
{
	class Blur : public IEffect
	{
	public:
		Blur();
		~Blur();
		void Initialise();
		void Update(float deltaTime, float totalTime);
		void Draw();

	private:
		D3D11RenderSystem& _renderSystem;
	};
}