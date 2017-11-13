#pragma once

#include <DirectXComponentsPCH.h>
#include "MeshLoader.h"
#include "Mesh.h"

namespace Fab
{
	class Model;

	class ModelLoader
	{
	public:
		ModelLoader();
		~ModelLoader();
		void Load(std::string fileName, std::vector<MeshData>& meshDatas);

	private:
		MeshLoader _meshLoader;
	};
}