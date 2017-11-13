#pragma once

#include <DirectXComponentsPCH.h>
#include "IEntity.h"

namespace Fab
{
	class Light : public IEntity
	{
	public:
		Light();
		virtual ~Light() = 0;
		void Initialise() override;
		void Draw() override;
		void Update(float deltaTime, float totalTime) override;
	};

	typedef std::shared_ptr<Light> LightPtr;
}
