#pragma once

#include <DirectXComponentsPCH.h>
#include "MeshLoader.h"

namespace Fab
{
	class MeshManager
	{
	public:
		MeshManager();
		~MeshManager();
		void     Initialise();
		void     Load(std::string fileName, std::string name);
		void     InsertMesh(std::string name, MeshPtr object);
		Mesh&    GetMesh(std::string name);
		MeshPtr  GetMeshPtr(std::string name);

	private:
		std::map<std::string, MeshPtr>   _meshes;
		MeshLoader                       _meshLoader;

	};
}