#pragma once

#include <DirectXComponentsPCH.h>
#include "Object.h"

namespace Fab
{
	class MeshLoader
	{
	public:
		MeshLoader();
		~MeshLoader();
		void Load(std::string fileName, Object& object);
	};
}