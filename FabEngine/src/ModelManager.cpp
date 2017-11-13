#include <DirectXComponentsPCH.h>
#include "ModelManager.h"

namespace Fab
{
	ModelManager::ModelManager()
	{
	}

	ModelManager::~ModelManager()
	{
	}

	void ModelManager::Initialise()
	{
	}

	void ModelManager::Load(std::string fileName, std::string name)
	{
		std::vector<MeshData> meshDatas;

		_modelLoader.Load(fileName, meshDatas);
		InsertModel(name, meshDatas);
	}

	void ModelManager::InsertModel(std::string name, std::vector<MeshData>& meshDatas)
	{
		_models.insert(std::pair<std::string, std::vector<MeshData>>(name, meshDatas));
	}

	void ModelManager::GetModel(std::string name, Model& model, XMFLOAT4 color)
	{
		auto found = _models.find(name);
		assert(found != _models.end());

		for (auto meshData : found->second)
		{
			MeshData tmp = meshData;

			for (auto i = tmp.Vertices.begin(); i != tmp.Vertices.end(); i++)
			{
				(*i).Color = color;
			}

			std::shared_ptr<Mesh> mesh(new Mesh());
			mesh->SetMeshData(tmp);
			mesh->Build();

			model.InsertMesh(mesh);
		}
	}
}