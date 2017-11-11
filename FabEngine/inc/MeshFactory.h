#pragma once

#include <DirectXComponentsPCH.h>
#include "IFactory.h"
#include "Mesh.h"

namespace Fab
{
	class D3D11RenderSystem;

	class MeshFactory : public IFactory
	{
	public:
		MeshFactory();
		~MeshFactory();
		void CreateCube(MeshPtr& mesh, float width, float height, float depth);
		void CreateSphere(MeshPtr& mesh, float radius, UINT sliceCount, UINT stackCount);
		void CreatePlane(MeshPtr& mesh, float width, float height);
		void CreateTriangle(MeshPtr& mesh, float width, float height);

	private:
		D3D11RenderSystem& _renderSystem;
	};
}