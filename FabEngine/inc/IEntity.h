#pragma once

#include <DirectXComponentsPCH.h>

namespace Fab
{
	class IEntity
	{
	public:
		IEntity();
		virtual ~IEntity() = 0;
		virtual void Draw() = 0;
		virtual void Update(float deltaTime, float totalTime) = 0;
	};
}