#include <DirectXComponentsPCH.h>
#include "SceneManager.h"

namespace Fab
{
	const XMFLOAT4 SceneManager::DefaultAmbientColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.4f);

	SceneManager::SceneManager()
		: _ambientColor(DefaultAmbientColor)
		, _renderSystem(D3D11RenderSystem::GetRenderSystem())
	{
	}

	SceneManager::~SceneManager()
	{
	}

	void SceneManager::InsertEntity(std::string name, IEntityPtr entity)
	{
		_entities.insert(std::pair<std::string, IEntityPtr>(name, std::move(entity)));
	}

	void SceneManager::Initialise()
	{
		UpdateAmbientColor();

		//_modelManager.Load("models/terminator.obj", "terminator");
		//_modelManager.Load("models/plan.blend", "plan");
		//_modelManager.Load("models/sphere.blend", "sphere");
		//_modelManager.Load("models/small-landscape.blend", "landscape");
		_modelManager.Load("models/big-monkey.blend", "monkey");

		{
			Model model;
			_modelManager.GetModel("monkey", model, Colors::Silver);
			model.GetMeshes().at(0)->Transform(
				XMMatrixRotationX(-2.0f*G_PI / 3.0f) *
				XMMatrixRotationY(XM_PI) *
				XMMatrixScaling(2.0f, 2.0f, 2.0f) *
				XMMatrixTranslation(0.0f, 0.0f, 0.0f)
			);
			InsertEntity("monkey", std::make_shared<Model>(model));
		}

		/*{
			Model model;
			_modelManager.GetModel("landscape", model, Colors::Silver);
			model.GetMeshes().at(0)->Transform(
				XMMatrixRotationX(-XM_PIDIV2) *
				XMMatrixScaling(1.0f, 1.0f, 1.0f)
			);
			InsertEntity("landscape", std::make_shared<Model>(model));
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

		for (auto& entity : _entities)
		{
			entity.second->Draw();
		}
	}

	void SceneManager::Update(float deltaTime, float totalTime)
	{
		Model& model = dynamic_cast<Model&>(GetEntity("monkey"));
		model.GetMeshes().at(0)->Transform(XMMatrixRotationY(deltaTime));

		if (totalTime > 1.0f)
			_camera.Update(deltaTime, totalTime);

		for (auto& entity : _entities)
		{
			entity.second->Update(deltaTime, totalTime);
		}
	}

	IEntity& SceneManager::GetEntity(std::string name)
	{
		auto found = _entities.find(name);
		assert(found != _entities.end());

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