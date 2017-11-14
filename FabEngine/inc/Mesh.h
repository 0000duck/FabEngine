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
		std::vector<VertexData>   Vertices;
		std::vector<WORD>         Indices;
		bool                      HasColor;

		MeshData()
			: HasColor(false)
		{
		}

		~MeshData()
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
		void        UpdateLocalPosition();
		void        Transform(XMMATRIX matrix);
		void        TransformLocal(XMMATRIX matrix);
		void        TransformAround(XMMATRIX matrix, XMFLOAT3 around);
		void        SetMeshData(MeshData& data);
		void	    SetSpecularColor(XMFLOAT4 specularColor);
		void        SetSpecularPower(float specularPower);
		MeshData&   GetMeshData();
		XMFLOAT4X4& GetWorld();

	public:
		static const XMFLOAT4 DefaultSpecularColor;
		static const float    DefaultSpecularPower;

	private:
		D3D11RenderSystem& _renderSystem;

		MeshData           _meshData;
		XMFLOAT4X4         _world;
		XMFLOAT3           _position;
		ShaderPtr          _shader;

		ID3D11Buffer*      _pVertexBuffer;
		ID3D11Buffer*      _pIndexBuffer;

		XMFLOAT4           _specularColor;
		float              _specularPower;
	};

	typedef std::shared_ptr<Mesh> MeshPtr;
}