#pragma once

#include <DirectXComponentsPCH.h>

namespace Fab
{
	class IEffect
	{
	public:
		IEffect();
		virtual ~IEffect() = 0;
		virtual void Initialise() = 0;
		virtual void Update(float deltaTime, float totalTime) = 0;
		virtual void Draw() = 0;
	};
}