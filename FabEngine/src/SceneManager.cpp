#include <DirectXComponentsPCH.h>
#include "SceneManager.h"

namespace Fab
{
	SceneManager::SceneManager()
	{
	}

	SceneManager::~SceneManager()
	{
	}

	void SceneManager::InsertEntity(std::string name, std::shared_ptr<IEntity> entity)
	{
		_entities.insert(std::pair<std::string, std::shared_ptr<IEntity>>(name, entity));
	}

	void SceneManager::Initialise()
	{
		{
			std::shared_ptr<Object> cube(new Object());
			XMFLOAT4X4& position = cube->GetPosition();
			XMStoreFloat4x4(&position, XMMatrixTranslation(0.0f, 0.0f, 0.0f));
			_objectFactory.CreateCube(cube, 1.0f, 1.0f, 1.0f);
			InsertEntity("cube-1", cube);
		}

		{
			std::shared_ptr<Object> cube(new Object());
			XMFLOAT4X4& position = cube->GetPosition();
			XMStoreFloat4x4(&position, XMMatrixTranslation(2.0f, 0.125f, 0.5f));
			_objectFactory.CreateCube(cube, 1.25f, 1.25f, 1.25f);
			InsertEntity("cube-2", cube);
		}

		{
			std::shared_ptr<Object> cube(new Object());
			XMFLOAT4X4& position = cube->GetPosition();
			XMStoreFloat4x4(&position, XMMatrixTranslation(4.0f, 0.0f, 0.0f));
			_objectFactory.CreateCube(cube, 1.0f, 1.0f, 1.0f);
			InsertEntity("cube-3", cube);
		}

		{
			std::shared_ptr<Object> cube(new Object());
			XMFLOAT4X4& position = cube->GetPosition();
			XMStoreFloat4x4(&position, XMMatrixTranslation(-3.5f, 0.0f, 0.0f));
			_objectFactory.CreateCube(cube, 1.0f, 1.0f, 1.0f);
			InsertEntity("cube-4", cube);
		}

		{
			std::shared_ptr<Object> sphere(new Object());
			XMFLOAT4X4& position = sphere->GetPosition();
			XMStoreFloat4x4(&position, XMMatrixTranslation(-2.0f, 0.0f, 0.0f));
			_objectFactory.CreateSphere(sphere, 0.5f, 20, 20);
			InsertEntity("sphere", sphere);
		}

		{
			std::shared_ptr<Object> ground(new Object());
			XMFLOAT4X4& position = ground->GetPosition();
			XMStoreFloat4x4(&position, XMMatrixRotationX(XM_PIDIV2) * XMMatrixTranslation(0.0f, -0.5f, 0.0f));
			_objectFactory.CreatePlane(ground, 9.5f, 5.0f);
			InsertEntity("ground", ground);
		}

		{
			_meshManager.Load("meshes/monkey.blend", "monkey");
			std::shared_ptr<Object> monkey = std::make_shared<Object>(*_meshManager.GetMeshPtr("monkey"));
			XMFLOAT4X4& position = monkey->GetPosition();
			XMStoreFloat4x4(&position, XMMatrixTranslation(-2.0f, 0.0f, 0.0f));
			//InsertEntity("monkey", monkey);
		}
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
}