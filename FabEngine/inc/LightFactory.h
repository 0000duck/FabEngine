#pragma once

#include <DirectXComponentsPCH.h>
#include "IFactory.h"
#include "Light.h"

namespace Fab
{
	class LightFactory: public IFactory
	{
	public:
		LightFactory();
		~LightFactory();
		void CreatePointLight(Light& light);
		void CreateSpotLight(Light& light);
		void CreateDirectionalLight(Light& light);
	};
}