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
		void                    Initialise();
		void                    Load(std::string fileName, std::string name);
		void                    InsertMesh(std::string name, std::shared_ptr<Object> object);
		Object&                 GetMesh(std::string name);
		std::shared_ptr<Object> GetMeshPtr(std::string name);

	private:
		std::map<std::string, std::shared_ptr<Object>> _meshes;
		MeshLoader                                     _meshLoader;

	};
}