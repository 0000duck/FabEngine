#pragma once

#include <DirectXComponentsPCH.h>
#include "IEntity.h"
#include "Vertex.h"

using namespace DirectX;

namespace Fab
{
#pragma region mesh_data
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
#pragma endregion mesh_data

#pragma region object
	class Object : public IEntity
	{
	public:
		Object();
		~Object();
		HRESULT     Build();
		void        Draw() override;
		void        Update(float deltaTime, float totalTime) override;
		MeshData&   GetMeshData();
		XMFLOAT4X4& GetPosition();

	public:
		MeshData                _meshData;
		XMFLOAT4X4              _position;
		std::shared_ptr<Shader> _shader;
	};
#pragma endregion mesh_data
}