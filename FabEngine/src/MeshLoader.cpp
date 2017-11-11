#include <DirectXComponentsPCH.h>
#include "MeshLoader.h"

namespace Fab
{
	MeshLoader::MeshLoader()
	{
	}

	MeshLoader::~MeshLoader()
	{
	}

	void MeshLoader::Load(std::string fileName, Mesh& mesh)
	{
		Assimp::Importer importer;

		const aiScene* scene = importer.ReadFile(fileName.c_str(),
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType);

		assert(scene != nullptr);

		if (scene->HasMeshes())
		{
			for (unsigned int i = 0; i < scene->mNumMeshes; i++)
			{
				aiMesh* aiMesh = scene->mMeshes[i];
				mesh.GetPosition();
			}
		}

		//mesh.Build();
	}
}