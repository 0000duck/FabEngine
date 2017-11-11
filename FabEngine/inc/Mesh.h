#pragma once

#include <DirectXComponentsPCH.h>
#include "IEntity.h"
#include "Vertex.h"

using namespace DirectX;

namespace Fab
{
	class D3D11RenderSystem;

	struct MeshData
	{
		std::vector<VertexColor> Vertices;
		std::vector<WORD>        Indices;
		ID3D11Buffer*            PVertexBuffer;
		ID3D11Buffer*            PIndexBuffer;

		MeshData()
			: PVertexBuffer(nullptr)
			, PIndexBuffer(nullptr)
		{
		}
	};

	class Mesh : public IEntity
	{
	public:
		Mesh();
		~Mesh();
		HRESULT     Build();
		void        Initialise() override;
		void        Draw() override;
		void        Update(float deltaTime, float totalTime) override;
		void        Transform(XMMATRIX matrix);
		MeshData&   GetMeshData();
		XMFLOAT4X4& GetPosition();

	public:
		D3D11RenderSystem&      _renderSystem;

		MeshData                _meshData;
		XMFLOAT4X4              _position;
		ShaderPtr               _shader;
	};

	typedef std::shared_ptr<Mesh> MeshPtr;
}