#include <DirectXComponentsPCH.h>
#include "SceneManager.h"

namespace Fab
{
	const XMFLOAT4 SceneManager::DefaultAmbientColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.3f);

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
		UpdateAmbientColor();

		_modelManager.Load("models/monkey-big.blend", "monkey");
		//_modelManager.Load("models/landscape-small.blend", "landscape");

		{
			Model model;
			_modelManager.GetModel("monkey", model, Colors::Silver);
			model.GetMeshes().at(0)->Transform(
				XMMatrixRotationX(-2.0f*G_PI / 3.0f) *
				XMMatrixRotationY(XM_PI) *
				XMMatrixScaling(2.0f, 2.0f, 2.0f) *
				XMMatrixTranslation(0.0f, 0.0f, 0.0f)
			);
			InsertModel("monkey", std::make_shared<Model>(model));
		}

		{
			DirectionalLight light;
			light.SetColor(XMFLOAT4(1.0f, 1.0f, 0.9f, 0.3f));
			light.ApplyRotation(XMMatrixRotationY(XM_PIDIV4));
			InsertLight("diffuse", std::make_shared<DirectionalLight>(light));
		}

		{
			PointLight light;
			light.SetColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 0.3f));
			light.SetPosition(XMFLOAT3(3.0f, 1.0f, -5.0f));
			light.SetRadius(16.0f);
			InsertLight("point", std::make_shared<PointLight>(light));
		}

		/*{
			Model model;
			_modelManager.GetModel("landscape", model, Colors::Silver);
			model.GetMeshes().at(0)->Transform(
				XMMatrixRotationX(-XM_PIDIV2) *
				XMMatrixScaling(16.0f, 16.0f, 16.0f)
			);
			InsertModel("landscape", std::make_shared<Model>(model));
		}*/

		/*
		{
			
			Model model;
			_modelManager.GetModel("terminator", model, Colors::Red);
			model.GetMeshes().at(0)->Transform(XMMatrixRotationY(XM_PI)*XMMatrixTranslation(0.0f, 0.0f, 1.0f));
			InsertEntity("terminator-1", std::make_shared<Model>(model));
		}

		{
			Model model;
			_modelManager.GetModel("terminator", model, Colors::Green);
			model.GetMeshes().at(0)->Transform(XMMatrixTranslation(2.2f, 0.0f, 1.0f));
			InsertEntity("terminator-2", std::make_shared<Model>(model));
		}

		{
			Model model;
			_modelManager.GetModel("terminator", model, Colors::Blue);
			model.GetMeshes().at(0)->Transform(XMMatrixTranslation(-2.2f, 0.0f, 1.0f));
			InsertEntity("terminator-3", std::make_shared<Model>(model));
		}

		{
			Model model;
			_modelManager.GetModel("sphere", model, Colors::Magenta);
			model.GetMeshes().at(0)->Transform(XMMatrixTranslation(0.0f, 0.5f, -2.5f));
			InsertEntity("sphere", std::make_shared<Model>(model));
		}

		{
			Model model;
			_modelManager.GetModel("plan", model, Colors::LightSteelBlue);
			model.GetMeshes().at(0)->Transform(
				XMMatrixScaling(5.0f, 3.0f, 1.0f) *
				XMMatrixRotationX(XM_PI) *
				XMMatrixTranslation(0.0f, 3.0f, 4.0f)
			);
			InsertEntity("wall-behind", std::make_shared<Model>(model));
		}

		{
			Model model;
			_modelManager.GetModel("plan", model, Colors::LightSteelBlue);
			model.GetMeshes().at(0)->Transform(
				XMMatrixScaling(4.0f, 3.0f, 1.0f) * 
				XMMatrixRotationY(XM_PIDIV2) * 
				XMMatrixTranslation(-5.0f, 3.0f, 0.0f)
			);
			InsertEntity("wall-left", std::make_shared<Model>(model));
		}

		{
			Model model;
			_modelManager.GetModel("plan", model, Colors::LightSteelBlue);
			model.GetMeshes().at(0)->Transform(
				XMMatrixScaling(4.0f, 3.0f, 1.0f) *
				XMMatrixRotationY(-XM_PIDIV2) *
				XMMatrixTranslation(5.0f, 3.0f, 0.0f)
			);
			InsertEntity("wall-right", std::make_shared<Model>(model));
		}

		{
			Model model;
			_modelManager.GetModel("plan", model, Colors::LightSteelBlue);
			model.GetMeshes().at(0)->Transform(
				XMMatrixScaling(5.0f, 4.0f, 1.0f) *
				XMMatrixRotationX(-XM_PIDIV2) *
				XMMatrixTranslation(0.0f, 0.0f, 0.0f)
			);
			InsertEntity("ground", std::make_shared<Model>(model));
		}

		{
			Model model;
			_modelManager.GetModel("plan", model, Colors::LightSteelBlue);
			model.GetMeshes().at(0)->Transform(
				XMMatrixScaling(5.0f, 4.0f, 1.0f) *
				XMMatrixRotationX(XM_PIDIV2) *
				XMMatrixTranslation(0.0f, 6.0f, 0.0f)
			);
			InsertEntity("roof", std::make_shared<Model>(model));
		}
		*/
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
		Model& model = dynamic_cast<Model&>(GetModel("monkey"));
		//model.GetMeshes().at(0)->Transform(XMMatrixRotationY(deltaTime));

		//PointLight& light = dynamic_cast<PointLight&>(GetLight("point"));

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