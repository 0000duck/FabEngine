#include <DirectXComponentsPCH.h>
#include "MeshManager.h"

namespace Fab
{
	MeshManager::MeshManager()
	{
	}

	MeshManager::~MeshManager()
	{
	}

	void MeshManager::Initialise()
	{
	}

	void MeshManager::Load(std::string fileName, std::string name)
	{
		MeshPtr mesh(new Mesh());
		_meshLoader.Load(fileName, *mesh);
		InsertMesh(name, mesh);
	}

	void MeshManager::InsertMesh(std::string name, MeshPtr mesh)
	{
		_meshes.insert(std::pair<std::string, MeshPtr>(name, mesh));
	}

	Mesh& MeshManager::GetMesh(std::string name)
	{
		auto found = _meshes.find(name);
		assert(found != _meshes.end());

		return *found->second;
	}

	MeshPtr MeshManager::GetMeshPtr(std::string name)
	{
		auto found = _meshes.find(name);
		assert(found != _meshes.end());

		return found->second;
	}
}