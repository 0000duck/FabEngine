#include <DirectXComponentsPCH.h>
#include "ModelLoader.h"

namespace Fab
{
	ModelLoader::ModelLoader()
	{
	}

	ModelLoader::~ModelLoader()
	{
	}

	void ModelLoader::Load(std::string fileName, std::vector<MeshData>& meshDatas)
	{
		Assimp::Importer importer;

		const aiScene* scene = importer.ReadFile(fileName.c_str(),
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType);

		assert(scene != nullptr);

		if (scene->HasMaterials())
		{
			
		}

		if (scene->HasMeshes())
		{
			for (unsigned int i = 0; i < scene->mNumMeshes; i++)
			{
				MeshData meshData;
				aiMesh* aiMesh = scene->mMeshes[i];
				_meshLoader.Load(aiMesh, meshData);
				meshDatas.push_back(meshData);
			}
		}
	}
}