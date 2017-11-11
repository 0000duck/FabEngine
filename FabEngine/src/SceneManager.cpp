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

	void SceneManager::InsertEntity(std::string name, IEntityPtr entity)
	{
		_entities.insert(std::pair<std::string, IEntityPtr>(name, entity));
	}

	void SceneManager::Initialise()
	{
		{
			ModelPtr model(new Model());
			MeshPtr cube(new Mesh());
			cube->Transform(XMMatrixTranslation(0.0f, 0.0f, 0.0f));
			_meshFactory.CreateCube(cube, 1.0f, 1.0f, 1.0f);
			model->InsertMesh(cube);
			InsertEntity("cube-1", model);
		}

		{
			ModelPtr model(new Model());
			MeshPtr cube(new Mesh());
			cube->Transform(XMMatrixTranslation(2.0f, 0.125f, 0.5f));
			_meshFactory.CreateCube(cube, 1.25f, 1.25f, 1.25f);
			model->InsertMesh(cube);
			InsertEntity("cube-2", model);
		}

		{
			ModelPtr model(new Model());
			MeshPtr cube(new Mesh());
			cube->Transform(XMMatrixTranslation(4.0f, 0.0f, 0.0f));
			_meshFactory.CreateCube(cube, 1.0f, 1.0f, 1.0f);
			model->InsertMesh(cube);
			InsertEntity("cube-3", model);
		}

		{
			ModelPtr model(new Model());
			MeshPtr cube(new Mesh());
			cube->Transform(XMMatrixTranslation(-3.5f, 0.0f, 0.0f));
			_meshFactory.CreateCube(cube, 1.0f, 1.0f, 1.0f);
			model->InsertMesh(cube);
			InsertEntity("cube-4", model);
		}

		{
			ModelPtr model(new Model());
			MeshPtr sphere(new Mesh());
			sphere->Transform(XMMatrixTranslation(-2.0f, 0.0f, 0.0f));
			_meshFactory.CreateSphere(sphere, 0.5f, 20, 20);
			model->InsertMesh(sphere);
			InsertEntity("sphere", model);
		}

		{
			ModelPtr model(new Model());
			MeshPtr ground(new Mesh());
			ground->Transform(XMMatrixRotationX(XM_PIDIV2) * XMMatrixTranslation(0.0f, -0.5f, 0.0f));
			_meshFactory.CreatePlane(ground, 9.5f, 5.0f);
			model->InsertMesh(ground);
			InsertEntity("ground", model);
		}

		{
			ModelPtr model(new Model());
			_meshManager.Load("meshes/monkey.blend", "monkey");
			MeshPtr monkey = std::make_shared<Mesh>(*_meshManager.GetMeshPtr("monkey"));
			monkey->Transform(XMMatrixTranslation(-2.0f, 0.0f, 0.0f));
			model->InsertMesh(monkey);
			//InsertEntity("monkey", model);
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