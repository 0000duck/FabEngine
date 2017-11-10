#pragma once

#include <DirectXComponentsPCH.h>
#include "MeshManager.h"
#include "IEntity.h"
#include "Camera.h"
#include "Light.h"
#include "Object.h"
#include "IFactory.h"
#include "LightFactory.h"
#include "ObjectFactory.h"

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
		void     InsertEntity(std::string name, std::shared_ptr<IEntity> entity);
		IEntity& GetEntity(std::string name);
		Camera&  GetCamera();

	private:
		std::map<std::string, std::shared_ptr<IEntity>> _entities;
		MeshManager   _meshManager;
		Camera        _camera;

		LightFactory  _lightFactory;
		ObjectFactory _objectFactory;
	};
}