#include <DirectXComponentsPCH.h>
#include "ObjectFactory.h"

namespace Fab
{
	ObjectFactory::ObjectFactory()
		: _renderSystem(D3D11RenderSystem::GetRenderSystem())
	{
	}

	ObjectFactory::~ObjectFactory()
	{
	}

	void ObjectFactory::CreateCube(std::shared_ptr<Object>& object, float width, float height, float depth)
	{
		VertexColor vertices[24];
		WORD indices[36];

		float w2 = 0.5f * width;
		float h2 = 0.5f * height;
		float d2 = 0.5f * depth;

		// Fill in the front face vertex data.
		vertices[0] = VertexColor(XMFLOAT3(-w2, -h2, -d2), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));
		vertices[1] = VertexColor(XMFLOAT3(-w2, +h2, -d2), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
		vertices[2] = VertexColor(XMFLOAT3(+w2, +h2, -d2), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f));
		vertices[3] = VertexColor(XMFLOAT3(+w2, -h2, -d2), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));

		// Fill in the back face vertex data.
		vertices[4] = VertexColor(XMFLOAT3(-w2, -h2, +d2), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));
		vertices[5] = VertexColor(XMFLOAT3(+w2, -h2, +d2), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
		vertices[6] = VertexColor(XMFLOAT3(+w2, +h2, +d2), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f));
		vertices[7] = VertexColor(XMFLOAT3(-w2, +h2, +d2), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));

		// Fill in the top face vertex data.
		vertices[8]  = VertexColor(XMFLOAT3(-w2, +h2, -d2), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));
		vertices[9]  = VertexColor(XMFLOAT3(-w2, +h2, +d2), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
		vertices[10] = VertexColor(XMFLOAT3(+w2, +h2, +d2), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f));
		vertices[11] = VertexColor(XMFLOAT3(+w2, +h2, -d2), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));

		// Fill in the bottom face vertex data.
		vertices[12] = VertexColor(XMFLOAT3(-w2, -h2, -d2), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));
		vertices[13] = VertexColor(XMFLOAT3(+w2, -h2, -d2), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
		vertices[14] = VertexColor(XMFLOAT3(+w2, -h2, +d2), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f));
		vertices[15] = VertexColor(XMFLOAT3(-w2, -h2, +d2), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));

		// Fill in the left face vertex data.
		vertices[16] = VertexColor(XMFLOAT3(-w2, -h2, +d2), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));
		vertices[17] = VertexColor(XMFLOAT3(-w2, +h2, +d2), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
		vertices[18] = VertexColor(XMFLOAT3(-w2, +h2, -d2), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f));
		vertices[19] = VertexColor(XMFLOAT3(-w2, -h2, -d2), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));

		// Fill in the right face vertex data.
		vertices[20] = VertexColor(XMFLOAT3(+w2, -h2, -d2), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));
		vertices[21] = VertexColor(XMFLOAT3(+w2, +h2, -d2), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
		vertices[22] = VertexColor(XMFLOAT3(+w2, +h2, +d2), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f));
		vertices[23] = VertexColor(XMFLOAT3(+w2, -h2, +d2), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));

		// Fill in the front face index data
		indices[0] = 0; indices[1] = 1; indices[2] = 2;
		indices[3] = 0; indices[4] = 2; indices[5] = 3;

		// Fill in the back face index data
		indices[6] = 4; indices[7] = 5; indices[8] = 6;
		indices[9] = 4; indices[10] = 6; indices[11] = 7;

		// Fill in the top face index data
		indices[12] = 8; indices[13] = 9; indices[14] = 10;
		indices[15] = 8; indices[16] = 10; indices[17] = 11;

		// Fill in the bottom face index data
		indices[18] = 12; indices[19] = 13; indices[20] = 14;
		indices[21] = 12; indices[22] = 14; indices[23] = 15;

		// Fill in the left face index data
		indices[24] = 16; indices[25] = 17; indices[26] = 18;
		indices[27] = 16; indices[28] = 18; indices[29] = 19;

		// Fill in the right face index data
		indices[30] = 20; indices[31] = 21; indices[32] = 22;
		indices[33] = 20; indices[34] = 22; indices[35] = 23;

		MeshData& meshData = object->GetMeshData();

		meshData.Vertices.insert(meshData.Vertices.end(), &vertices[0], &vertices[24]);
		meshData.Indices.insert(meshData.Indices.end(), &indices[0], &indices[36]);

		object->Build();
	}

	void ObjectFactory::CreateSphere(std::shared_ptr<Object>& object, float radius, UINT sliceCount, UINT stackCount)
	{
		MeshData& meshData = object->GetMeshData();
		VertexColor topVertex(XMFLOAT3(0.0f, +radius, 0.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));
		VertexColor bottomVertex(XMFLOAT3(0.0f, -radius, 0.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));

		meshData.Vertices.push_back(topVertex);

		float phiStep = XM_PI / stackCount;
		float thetaStep = 2.0f*XM_PI / sliceCount;

		// Compute vertices for each stack ring (do not count the poles as rings).
		for (UINT i = 1; i <= stackCount - 1; ++i)
		{
			float phi = i*phiStep;

			// Vertices of ring.
			for (UINT j = 0; j <= sliceCount; ++j)
			{
				float theta = j*thetaStep;

				VertexColor v;

				// spherical to cartesian
				v.Position.x = radius*sinf(phi)*cosf(theta);
				v.Position.y = radius*cosf(phi);
				v.Position.z = radius*sinf(phi)*sinf(theta);

				// Partial derivative of P with respect to theta
				v.Color.x = -radius*sinf(phi)*sinf(theta);
				v.Color.y = 0.0f;
				v.Color.z = +radius*sinf(phi)*cosf(theta);
				v.Color.w = 1.0f;

				XMVECTOR T = XMLoadFloat4(&v.Color);
				XMStoreFloat4(&v.Color, XMVector3Normalize(T));

				meshData.Vertices.push_back(v);
			}
		}

		meshData.Vertices.push_back(bottomVertex);

		// Compute indices for top stack.  The top stack was written first to the vertex buffer
		// and connects the top pole to the first ring.
		for (UINT i = 1; i <= sliceCount; ++i)
		{
			meshData.Indices.push_back(0);
			meshData.Indices.push_back(i + 1);
			meshData.Indices.push_back(i);
		}

		// Compute indices for inner stacks (not connected to poles).
		// Offset the indices to the index of the first vertex in the first ring.

		// This is just skipping the top pole vertex.
		UINT baseIndex = 1;
		UINT ringVertexCount = sliceCount + 1;
		for (UINT i = 0; i < stackCount - 2; ++i)
		{
			for (UINT j = 0; j < sliceCount; ++j)
			{
				meshData.Indices.push_back(baseIndex + i*ringVertexCount + j);
				meshData.Indices.push_back(baseIndex + i*ringVertexCount + j + 1);
				meshData.Indices.push_back(baseIndex + (i + 1)*ringVertexCount + j);

				meshData.Indices.push_back(baseIndex + (i + 1)*ringVertexCount + j);
				meshData.Indices.push_back(baseIndex + i*ringVertexCount + j + 1);
				meshData.Indices.push_back(baseIndex + (i + 1)*ringVertexCount + j + 1);
			}
		}

		// Compute indices for bottom stack.  The bottom stack was written last to the vertex buffer
		// and connects the bottom pole to the bottom ring.

		// South pole vertex was added last.
		UINT southPoleIndex = (UINT)meshData.Vertices.size() - 1;

		// Offset the indices to the index of the first vertex in the last ring.
		baseIndex = southPoleIndex - ringVertexCount;

		for (UINT i = 0; i < sliceCount; ++i)
		{
			meshData.Indices.push_back(southPoleIndex);
			meshData.Indices.push_back(baseIndex + i);
			meshData.Indices.push_back(baseIndex + i + 1);
		}

		object->Build();
	}

	void ObjectFactory::CreatePlane(std::shared_ptr<Object>& object, float width, float height)
	{
		VertexColor vertices[4];
		WORD indices[6];

		float w2 = 0.5f * width;
		float h2 = 0.5f * height;

		vertices[0] = VertexColor(XMFLOAT3(-w2, h2, 0.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));
		vertices[1] = VertexColor(XMFLOAT3(w2, h2, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
		vertices[2] = VertexColor(XMFLOAT3(-w2, -h2, 0.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f));
		vertices[3] = VertexColor(XMFLOAT3(w2, -h2, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));

		indices[0] = 0; indices[1] = 1; indices[2] = 2;
		indices[3] = 2; indices[4] = 1; indices[5] = 3;

		MeshData& meshData = object->GetMeshData();

		meshData.Vertices.insert(meshData.Vertices.end(), &vertices[0], &vertices[4]);
		meshData.Indices.insert(meshData.Indices.end(), &indices[0], &indices[6]);

		object->Build();
	}

	void ObjectFactory::CreateTriangle(std::shared_ptr<Object>& object, float width, float height)
	{
		VertexColor vertices[3];
		WORD indices[3];

		float w2 = 0.5f * width;
		float h2 = 0.5f * height;

		vertices[0] = VertexColor(XMFLOAT3(-w2, h2, 0.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));
		vertices[1] = VertexColor(XMFLOAT3(w2, h2, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
		vertices[2] = VertexColor(XMFLOAT3(-w2, -h2, 0.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f));

		indices[0] = 0; indices[1] = 1; indices[2] = 2;

		MeshData& meshData = object->GetMeshData();

		meshData.Vertices.insert(meshData.Vertices.end(), &vertices[0], &vertices[3]);
		meshData.Indices.insert(meshData.Indices.end(), &indices[0], &indices[3]);

		object->Build();
	}

	void ObjectFactory::CreateMesh(std::shared_ptr<Object>& object)
	{
	}
}