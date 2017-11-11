#pragma once

#include <DirectXComponentsPCH.h>
#include "IEntity.h"

using namespace DirectX;

namespace Fab
{
	class Particle: public IEntity
	{
	public:
		Particle();
		~Particle();
		void Initialise() override;
		void Draw() override;
		void Update(float deltaTime, float totalTime) override;

	private:
		//position
		//oldPosition
		//velocity
		//energy
		//force
		//in use
		//alive
	};
}