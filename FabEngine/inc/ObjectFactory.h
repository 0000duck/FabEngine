#pragma once

#include <DirectXComponentsPCH.h>
#include "IFactory.h"
#include "Object.h"

namespace Fab
{
	class D3D11RenderSystem;

	class ObjectFactory : public IFactory
	{
	public:
		ObjectFactory();
		~ObjectFactory();
		void CreateCube(std::shared_ptr<Object>& object, float width, float height, float depth);
		void CreateSphere(std::shared_ptr<Object>& object, float radius, UINT sliceCount, UINT stackCount);
		void CreatePlane(std::shared_ptr<Object>& object, float width, float height);
		void CreateTriangle(std::shared_ptr<Object>& object, float width, float height);
		void CreateMesh(std::shared_ptr<Object>& object);

	private:
		D3D11RenderSystem& _renderSystem;
	};
}