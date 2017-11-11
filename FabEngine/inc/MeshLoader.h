#pragma once

#include <DirectXComponentsPCH.h>
#include "Mesh.h"

namespace Fab
{
	class MeshLoader
	{
	public:
		MeshLoader();
		~MeshLoader();
		void Load(std::string fileName, Mesh& object);
	};
}