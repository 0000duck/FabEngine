#pragma once

#include <DirectXComponentsPCH.h>
#include "MeshManager.h"
#include "IEntity.h"
#include "Camera.h"
#include "Light.h"
#include "Model.h"
#include "Mesh.h"
#include "IFactory.h"
#include "LightFactory.h"
#include "MeshFactory.h"

namespace Fab
{
	class SceneManager
	{
	public:
		SceneManager();
		~SceneManager();
		void     Initialise();
		void     Draw();
		void     Update(float deltaTime, float totalTime);
		void     InsertEntity(std::string name, IEntityPtr entity);
		IEntity& GetEntity(std::string name);
		Camera&  GetCamera();

	private:
		std::map<std::string, IEntityPtr> _entities;
		MeshManager                       _meshManager;
		Camera                            _camera;

		LightFactory                      _lightFactory;
		MeshFactory                       _meshFactory;
	};
}