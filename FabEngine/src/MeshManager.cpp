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
		std::shared_ptr<Object> object(new Object());
		_meshLoader.Load(fileName, *object);
		InsertMesh(name, object);
	}

	void MeshManager::InsertMesh(std::string name, std::shared_ptr<Object> object)
	{
		_meshes.insert(std::pair<std::string, std::shared_ptr<Object>>(name, object));
	}

	Object& MeshManager::GetMesh(std::string name)
	{
		auto found = _meshes.find(name);
		assert(found != _meshes.end());

		return *found->second;
	}

	std::shared_ptr<Object> MeshManager::GetMeshPtr(std::string name)
	{
		auto found = _meshes.find(name);
		assert(found != _meshes.end());

		return found->second;
	}
}