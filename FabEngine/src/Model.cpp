#include <DirectXComponentsPCH.h>
#include "Model.h"

namespace Fab
{
	Model::Model()
	{
	}

	Model::~Model()
	{
	}

	void Model::Initialise()
	{
	}

	void Model::Draw()
	{
		for (auto mesh : _meshes)
		{
			mesh->Draw();
		}
	}

	void Model::Update(float deltaTime, float totalTime)
	{
		for (auto mesh : _meshes)
		{
			mesh->Update(deltaTime, totalTime);
		}
	}

	void Model::InsertMesh(MeshPtr mesh)
	{
		_meshes.push_back(mesh);
	}

	std::vector<MeshPtr>& Model::GetMeshes()
	{
		return _meshes;
	}
}
