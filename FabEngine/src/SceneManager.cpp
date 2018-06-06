#include <DirectXComponentsPCH.h>
#include "SceneManager.h"

namespace Fab
{
	const XMFLOAT4 SceneManager::DefaultAmbientColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.2f);

	SceneManager::SceneManager()
		: _ambientColor(DefaultAmbientColor)
		, _renderSystem(D3D11RenderSystem::GetRenderSystem())
		, _ambientColorSkyDome(XMFLOAT4(1.0f, 1.0f, 1.0f, 0.75f))
	{
	}

	SceneManager::~SceneManager()
	{
	}

	void SceneManager::InsertModel(std::string name, ModelPtr model)
	{
		_models.insert(std::pair<std::string, ModelPtr>(name, std::move(model)));
	}

	void SceneManager::InsertLight(std::string name, LightPtr light)
	{
		_lights.insert(std::pair<std::string, LightPtr>(name, std::move(light)));
	}

	void SceneManager::Initialise()
	{
		tinyxml2::XMLDocument doc;
		doc.LoadFile("data/scene.xml");

		tinyxml2::XMLElement* sceneElement = doc.FirstChildElement("scene");

		//load ambient color
		{
			tinyxml2::XMLElement* colorElement = sceneElement->FirstChildElement("ambient")->FirstChildElement("color");
			_ambientColor.x = colorElement->FloatAttribute("r", _ambientColor.x);
			_ambientColor.y = colorElement->FloatAttribute("g", _ambientColor.y);
			_ambientColor.z = colorElement->FloatAttribute("b", _ambientColor.z);
			_ambientColor.w = colorElement->FloatAttribute("i", _ambientColor.w);
		}

		//load fog
		{
			tinyxml2::XMLElement* effectsElement = sceneElement->FirstChildElement("effects");

			for (tinyxml2::XMLElement* effectElement = effectsElement->FirstChildElement("effect"); effectElement != nullptr; effectElement = effectElement->NextSiblingElement())
			{
				std::string type = effectElement->Attribute("type");

				if (type == "fog")
				{
					float fogStart = effectElement->FloatAttribute("start");
					float fogRange = effectElement->FloatAttribute("range");

					tinyxml2::XMLElement* colorElement = effectElement->FirstChildElement("color");
					XMFLOAT4 fogColor;

					fogColor.x = colorElement->FloatAttribute("r", fogColor.x);
					fogColor.y = colorElement->FloatAttribute("g", fogColor.y);
					fogColor.z = colorElement->FloatAttribute("b", fogColor.z);
					fogColor.w = colorElement->FloatAttribute("i", fogColor.w);

					Fog fog(fogColor, fogStart, fogRange);
				}
				else if (type == "filter")
				{
					std::string filterType = effectElement->Attribute("filter");
					Filter filter(filterType);
				}
				else if (type == "blur")
				{
				}
			}
		}

		//load textures
		{
			tinyxml2::XMLElement* texturesElement = sceneElement->FirstChildElement("textures");

			for (tinyxml2::XMLElement* textureElement = texturesElement->FirstChildElement("texture"); textureElement != nullptr; textureElement = textureElement->NextSiblingElement())
			{
				const char* file = textureElement->Attribute("file");
				const char* name = textureElement->Attribute("name");
				int slot         = textureElement->IntAttribute("slot");

				WCHAR* fileW;

				int nChars = MultiByteToWideChar(CP_ACP, 0, file, -1, NULL, 0);
				fileW = new WCHAR[nChars];
				MultiByteToWideChar(CP_ACP, 0, file, -1, (LPWSTR)fileW, nChars);

				_textureManager.Load(fileW, std::string(name), slot);

				delete[] fileW;
			}
		}

		//loads 3D objects
		{
			tinyxml2::XMLElement* modelsElement = sceneElement->FirstChildElement("objects");

			for (tinyxml2::XMLElement* modelElement = modelsElement->FirstChildElement("object"); modelElement != nullptr; modelElement = modelElement->NextSiblingElement())
			{
				std::string file = modelElement->Attribute("file");
				std::string name = modelElement->Attribute("name");

				_modelManager.Load(file, name);
			}
		}

		//load models
		{
			tinyxml2::XMLElement* modelsElement = sceneElement->FirstChildElement("models");

			for (tinyxml2::XMLElement* modelElement = modelsElement->FirstChildElement("model"); modelElement != nullptr; modelElement = modelElement->NextSiblingElement())
			{
				std::string object = modelElement->Attribute("object");
				std::string name = modelElement->Attribute("name");
				const char* texture = modelElement->Attribute("texture");
				const char* specular = modelElement->Attribute("specular");
				const char* normal = modelElement->Attribute("normal");

				{
					Model model;
					_modelManager.GetModel(object, model, Colors::Silver);

					if (texture != nullptr && strlen(texture) > 0)
					{
						model.GetMeshes().at(0)->SetTexture(_textureManager.GetTexturePtr(texture));
					}

					if (specular != nullptr && strlen(specular) > 0)
					{
						model.GetMeshes().at(0)->SetSpecular(_textureManager.GetTexturePtr(specular));
					}

					if (normal != nullptr && strlen(normal) > 0)
					{
						model.GetMeshes().at(0)->SetNormal(_textureManager.GetTexturePtr(normal));
					}

					XMMATRIX matrix = XMMatrixIdentity();

					for (tinyxml2::XMLElement* transformationElement = modelElement->FirstChildElement("transformations")->FirstChildElement("transformation"); transformationElement != nullptr; transformationElement = transformationElement->NextSiblingElement())
					{
						std::string type = transformationElement->Attribute("type");

						if (type == "rotation-x")
						{
							float x = transformationElement->FloatAttribute("x");
							matrix *= XMMatrixRotationX(x);
						}
						else if (type == "rotation-y")
						{
							float y = transformationElement->FloatAttribute("y");
							matrix *= XMMatrixRotationY(y);
						}
						else if (type == "rotation-z")
						{
							float z = transformationElement->FloatAttribute("z");
							matrix *= XMMatrixRotationZ(z);
						}
						else if (type == "translation")
						{
							float x = transformationElement->FloatAttribute("x");
							float y = transformationElement->FloatAttribute("y");
							float z = transformationElement->FloatAttribute("z");

							matrix *= XMMatrixTranslation(x, y, z);
						}
						else if (type == "scaling")
						{
							float x = transformationElement->FloatAttribute("x");
							float y = transformationElement->FloatAttribute("y");
							float z = transformationElement->FloatAttribute("z");

							matrix *= XMMatrixScaling(x, y, z);
						}
					}

					//Parameters
					tinyxml2::XMLElement* specularElement = modelElement->FirstChildElement("parameters")->FirstChildElement("specular");

					XMFLOAT4 specularColor;
					float    specularPower;

					specularColor.x = specularElement->FloatAttribute("r");
					specularColor.y = specularElement->FloatAttribute("g");
					specularColor.z = specularElement->FloatAttribute("b");
					specularColor.w = specularElement->FloatAttribute("i");

					specularPower = specularElement->FloatAttribute("p");

					model.GetMeshes().at(0)->SetSpecularColor(specularColor);
					model.GetMeshes().at(0)->SetSpecularPower(specularPower);

					model.GetMeshes().at(0)->Transform(matrix);
					InsertModel(name, std::make_shared<Model>(model));
				}
			}
		}

		//Load lights
		{
			tinyxml2::XMLElement* lightsElement = sceneElement->FirstChildElement("lights");

			for (tinyxml2::XMLElement* lightElement = lightsElement->FirstChildElement("light"); lightElement != nullptr; lightElement = lightElement->NextSiblingElement())
			{
				std::string type   = lightElement->Attribute("type");
				std::string name   = lightElement->Attribute("name");
				const char* object = lightElement->Attribute("object");

				XMFLOAT4 color;

				tinyxml2::XMLElement* colorElement = lightElement->FirstChildElement("color");
				color.x = colorElement->FloatAttribute("r");
				color.y = colorElement->FloatAttribute("g");
				color.z = colorElement->FloatAttribute("b");
				color.w = colorElement->FloatAttribute("i");

				if (type == "directional")
				{
					DirectionalLight light;
					light.SetColor(color);

					const char* rotationAxe = lightElement->Attribute("rotation-axe");

					if (rotationAxe != nullptr)
					{
						float rotation = lightElement->FloatAttribute("rotation-value");

						if (rotationAxe == "x")
						{
							light.ApplyRotation(XMMatrixRotationX(rotation));
						}
						else if (rotationAxe == "y")
						{
							light.ApplyRotation(XMMatrixRotationY(rotation));
						}
						else
						{
							light.ApplyRotation(XMMatrixRotationZ(rotation));
						}
					}

					InsertLight(name, std::make_shared<DirectionalLight>(light));
				}
				else if (type == "point")
				{
					XMFLOAT4 specularColor(1.0f, 1.0f, 0.5f, 1.0f);
					float    specularPower = 64.0f;

					PointLight light;
					light.SetColor(color);
					light.SetRadius(20.0f);

					Model model;
					_modelManager.GetModel(object, model, Colors::Yellow);
					model.GetMeshes().at(0)->SetSpecularColor(specularColor);
					model.GetMeshes().at(0)->SetSpecularPower(specularPower);
					light.SetModel(std::make_shared<Model>(model));
					light.Initialise();

					InsertLight("point", std::make_shared<PointLight>(light));
				}
			}
		}

		//Load SkyDome
		Model model;
		_modelManager.GetModel("sky", model, Colors::Silver);
		model.GetMeshes().at(0)->SetTexture(_textureManager.GetTexturePtr("sky-diffuse"));
		model.GetMeshes().at(0)->SetSpecularColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f));
		model.GetMeshes().at(0)->SetSpecularPower(8.0f);

		XMMATRIX matrix = XMMatrixIdentity();
		matrix *= XMMatrixScaling(12.0f, 12.0f, 12.0f);
		matrix *= XMMatrixTranslation(0.0f, 24.0f, 0.0f);
		model.GetMeshes().at(0)->Transform(matrix);
		
		_positionSkyBox = _camera.GetPosition();
		_skyDome = std::make_shared<Model>(model);

		//AmbientColor
		UpdateAmbientColor();
	}

	void SceneManager::Draw()
	{
		_camera.Draw();
		_frustum.BuildFrustum(_camera.GetView(), _camera.GetProjection(), _camera.GetFarZ());
		
		//Draw light
		for (auto& light : _lights)
		{
			light.second->Draw();
		}

		//Draw models
		for (auto& entity : _models)
		{
			std::vector<MeshPtr>& meshes = entity.second->GetMeshes();

			for (auto mesh : meshes)
			{
				if (_frustum.CheckSphere(_camera.GetView(), _camera.GetProjection(), mesh->GetPosition(), 0.5f))
				{
					mesh->Draw();
				}
			}
		}

		//Draw light models
		for (auto& light : _lights)
		{
			ModelPtr& model = light.second->GetModelPtr();

			if (model != nullptr)
			{
				std::vector<MeshPtr>& meshes = model->GetMeshes();

				for (auto mesh : meshes)
				{
					if (_frustum.CheckSphere(_camera.GetView(), _camera.GetProjection(), mesh->GetPosition(), 0.5f))
					{
						mesh->Draw();
					}
				}
			}
		}

		//Draw skydome
		std::vector<MeshPtr>& meshes = _skyDome->GetMeshes();

		UpdateAmbientColorSkyDome();

		for (auto mesh : meshes)
		{
			mesh->Draw();
		}

		UpdateAmbientColor();
	}

	void SceneManager::Update(float deltaTime, float totalTime)
	{
		if (totalTime > 1.0f)
			_camera.Update(deltaTime, totalTime);

		for (auto& light : _lights)
		{
			light.second->Update(deltaTime, totalTime);
		}

		for (auto& entity : _models)
		{
			entity.second->Update(deltaTime, totalTime);
		}

		XMFLOAT3 position = _camera.GetPosition();
		XMFLOAT3 oldPosition = _camera.GetOldPosition();

		if (_positionSkyBox.x != position.x || _positionSkyBox.y != position.y || _positionSkyBox.z != position.z)
		{
			_positionSkyBox = position;

			XMMATRIX matrix = XMMatrixIdentity();
			matrix *= XMMatrixTranslation(position.x - oldPosition.x, position.y - oldPosition.y, position.z - oldPosition.z);
			_skyDome->GetMeshes().at(0)->Transform(matrix);
		}
	}

	Model& SceneManager::GetModel(std::string name)
	{
		auto found = _models.find(name);
		assert(found != _models.end());

		return *found->second;
	}

	Light& SceneManager::GetLight(std::string name)
	{
		auto found = _lights.find(name);
		assert(found != _lights.end());

		return *found->second;
	}

	Camera&  SceneManager::GetCamera()
	{
		return _camera;
	}

	void SceneManager::UpdateAmbientColor()
	{
		FrameConstantBuffer* pFrameConstantBufferUpdate = _renderSystem.GetFrameConstantBufferUpdate();
		ID3D11Buffer** pFrameConstantBuffer = _renderSystem.GetFrameConstantBuffer();
		ID3D11DeviceContext** pContext = _renderSystem.GetPImmediateContext();

		pFrameConstantBufferUpdate->AmbientColor = _ambientColor;
		(*pContext)->UpdateSubresource(*pFrameConstantBuffer, 0, nullptr, pFrameConstantBufferUpdate, 0, 0);
	}

	void SceneManager::UpdateAmbientColorSkyDome()
	{
		FrameConstantBuffer* pFrameConstantBufferUpdate = _renderSystem.GetFrameConstantBufferUpdate();
		ID3D11Buffer** pFrameConstantBuffer = _renderSystem.GetFrameConstantBuffer();
		ID3D11DeviceContext** pContext = _renderSystem.GetPImmediateContext();

		pFrameConstantBufferUpdate->AmbientColor = _ambientColorSkyDome;
		(*pContext)->UpdateSubresource(*pFrameConstantBuffer, 0, nullptr, pFrameConstantBufferUpdate, 0, 0);
	}
}