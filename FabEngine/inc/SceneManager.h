#pragma once

#include <DirectXComponentsPCH.h>
#include "ModelManager.h"
#include "IEntity.h"
#include "Camera.h"
#include "Light.h"
#include "Model.h"
#include "Mesh.h"

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
		void     UpdateAmbientColor();

	public:
		static const XMFLOAT4 DefaultAmbientColor;

	private:
		D3D11RenderSystem&                _renderSystem;
		ModelManager                      _modelManager;

		std::map<std::string, IEntityPtr> _entities;
		std::map<std::string, IEntityPtr> _lights;
		Camera                            _camera;

		XMFLOAT4                          _ambientColor;
	};
}