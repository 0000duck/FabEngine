#pragma once

#include <DirectXComponentsPCH.h>
#include "ModelLoader.h"
#include "Model.h"

namespace Fab
{
	class ModelManager
	{
	public:
		ModelManager();
		~ModelManager();
		void Initialise();
		void Load(std::string fileName, std::string name);
		void InsertModel(std::string name, std::vector<MeshData>& meshDatas);
		void GetModel(std::string name, Model& model, XMFLOAT4 color);

	private:
		std::map<std::string, std::vector<MeshData>> _models;
		ModelLoader                                  _modelLoader;

	};
}