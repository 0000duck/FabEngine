#pragma once

#include <DirectXComponentsPCH.h>
#include "Particle.h"

using namespace DirectX;

namespace Fab
{
	class ParticleSystem
	{
	public:
		ParticleSystem();
		~ParticleSystem();

		//init()

	protected:
		std::list<Particle> _particle;
		//emitter position
	};
}