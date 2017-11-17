#pragma once

#include <DirectXComponentsPCH.h>
#include "ModelManager.h"
#include "TextureManager.h"
#include "IEntity.h"
#include "Camera.h"
#include "Light.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Model.h"
#include "Mesh.h"
#include "Fog.h"
#include "Filter.h"
#include "Blur.h"
#include "Frustum.h"

namespace Fab
{
	enum class FilterType
	{
		NONE, INVERT, SEPIA, GREYSCALE
	};

	class SceneManager
	{
	public:
		SceneManager();
		~SceneManager();
		void     Initialise();
		void     Draw();
		void     Update(float deltaTime, float totalTime);
		void     InsertModel(std::string name, ModelPtr model);
		void     InsertLight(std::string name, LightPtr light);
		Model&   GetModel(std::string name);
		Light&   GetLight(std::string name);
		Camera&  GetCamera();

	private:
		void     UpdateAmbientColor();

	public:
		static const XMFLOAT4 DefaultAmbientColor;

	private:
		D3D11RenderSystem&              _renderSystem;
		ModelManager                    _modelManager;
		TextureManager                  _textureManager;
		Frustum                         _frustum;

		std::map<std::string, ModelPtr> _models;
		std::map<std::string, LightPtr> _lights;
		Camera                          _camera;

		XMFLOAT4                        _ambientColor;
	};
}