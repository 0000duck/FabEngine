#pragma once

#include <DirectXComponentsPCH.h>
#include "IEntity.h"

namespace Fab
{
	class Light : public IEntity
	{
	public:
		Light();
		~Light();
		void Initialise() override;
		void Draw() override;
		void Update(float deltaTime, float totalTime) override;
	};
}
