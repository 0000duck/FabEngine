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

	void MeshLoader::Load(std::string fileName, Object& object)
	{
		Assimp::Importer importer;

		const aiScene* scene = importer.ReadFile(fileName.c_str(),
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType);

		assert(scene);

		//object.Build();
	}
}