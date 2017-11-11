#pragma once

#include <DirectXComponentsPCH.h>
#include "IEntity.h"
#include "Mesh.h"

using namespace DirectX;

namespace Fab
{
	class Model: public IEntity
	{
	public:
		Model();
		~Model();
		void                  Initialise() override;
		void                  Draw() override;
		void                  Update(float deltaTime, float totalTime) override;
		void                  InsertMesh(MeshPtr mesh);
		std::vector<MeshPtr>& GetMeshes();

	private:
		Model(const Model& model) = delete;
		Model& operator=(const Model& model) = delete;

	private:
		std::vector<MeshPtr> _meshes;
	};

	typedef std::shared_ptr<Model> ModelPtr;
}