#include <DirectXComponentsPCH.h>
#include "SceneManager.h"

namespace Fab
{
	const XMFLOAT4 SceneManager::DefaultAmbientColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.2f);

	SceneManager::SceneManager()
		: _ambientColor(DefaultAmbientColor)
		, _renderSystem(D3D11RenderSystem::GetRenderSystem())
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

		//load textures
		{
			tinyxml2::XMLElement* texturesElement = sceneElement->FirstChildElement("textures");

			for (tinyxml2::XMLElement* textureElement = texturesElement->FirstChildElement("texture"); textureElement != nullptr; textureElement = textureElement->NextSiblingElement())
			{
				const char* file = textureElement->Attribute("file");
				const char* name = textureElement->Attribute("name");

				WCHAR* fileW;

				int nChars = MultiByteToWideChar(CP_ACP, 0, file, -1, NULL, 0);
				fileW = new WCHAR[nChars];
				MultiByteToWideChar(CP_ACP, 0, file, -1, (LPWSTR)fileW, nChars);

				_textureManager.Load(fileW, std::string(name));

				delete[] fileW;
			}
		}

		//load models
		{
			tinyxml2::XMLElement* modelsElement = sceneElement->FirstChildElement("models");

			for (tinyxml2::XMLElement* modelElement = modelsElement->FirstChildElement("model"); modelElement != nullptr; modelElement = modelElement->NextSiblingElement())
			{
				std::string file = modelElement->Attribute("file");
				std::string name = modelElement->Attribute("name");
				const char* texture = modelElement->Attribute("texture");

				_modelManager.Load(file, name);

				{
					Model model;
					_modelManager.GetModel(name, model, Colors::Silver);

					if (texture != nullptr)
					{
						model.GetMeshes().at(0)->SetTexture(_textureManager.GetTexturePtr(texture));
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
				std::string type = lightElement->Attribute("type");
				std::string name = lightElement->Attribute("name");

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
					PointLight light;
					light.SetColor(color);
					light.SetRadius(20.0f);
					InsertLight("point", std::make_shared<PointLight>(light));
				}
			}
		}

		UpdateAmbientColor();
	}

	void SceneManager::Draw()
	{
		_camera.Draw();

		for (auto& light : _lights)
		{
			light.second->Draw();
		}

		for (auto& entity : _models)
		{
			entity.second->Draw();
		}
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
}