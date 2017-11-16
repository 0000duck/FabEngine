#include <DirectXComponentsPCH.h>
#include "MeshLoader.h"

namespace Fab
{
	MeshLoader::MeshLoader()
	{
	}

	MeshLoader::~MeshLoader()
	{
	}

	void MeshLoader::Load(aiMesh* aiMesh, MeshData& meshData)
	{
		//Vertices
		meshData.Vertices.reserve(aiMesh->mNumVertices);
		for (UINT i = 0; i < aiMesh->mNumVertices; i++)
		{
			VertexData vertexData;
			vertexData.Position = XMFLOAT3(reinterpret_cast<const float*>(&aiMesh->mVertices[i]));
			meshData.Vertices.push_back(vertexData);
		}

		//Normals
		if (aiMesh->HasNormals())
		{
			for (UINT i = 0; i < aiMesh->mNumVertices; i++)
			{
				meshData.Vertices.at(i).Normal = XMFLOAT3(reinterpret_cast<const float*>(&aiMesh->mNormals[i]));
			}
		}

		// Faces
		if (aiMesh->HasFaces())
		{
			unsigned int faceCount = aiMesh->mNumFaces;
			int k = 0;

			for (UINT i = 0; i < faceCount; i++)
			{
				aiFace* face = &aiMesh->mFaces[i];

				for (UINT j = 0; j < face->mNumIndices; j++)
				{
					meshData.Indices.push_back((WORD)face->mIndices[j]);
				}

				// Edges of the triangle : postion delta
				XMVECTOR deltaPos1Vec = XMLoadFloat3(&meshData.Vertices.at(meshData.Indices.at(k + 1)).Position) - XMLoadFloat3(&meshData.Vertices.at(meshData.Indices.at(k)).Position);
				XMVECTOR deltaPos2Vec = XMLoadFloat3(&meshData.Vertices.at(meshData.Indices.at(k + 2)).Position) - XMLoadFloat3(&meshData.Vertices.at(meshData.Indices.at(k)).Position);

				// UV delta
				XMVECTOR deltaUV1Vec = XMLoadFloat3(&meshData.Vertices.at(meshData.Indices.at(k + 1)).Normal) - XMLoadFloat3(&meshData.Vertices.at(meshData.Indices.at(k)).Normal);
				XMVECTOR deltaUV2Vec = XMLoadFloat3(&meshData.Vertices.at(meshData.Indices.at(k + 2)).Normal) - XMLoadFloat3(&meshData.Vertices.at(meshData.Indices.at(k)).Normal);
			
				XMFLOAT3 deltaPos1;
				XMFLOAT3 deltaPos2;
				XMFLOAT3 deltaUV1;
				XMFLOAT3 deltaUV2;

				XMStoreFloat3(&deltaPos1, deltaPos1Vec);
				XMStoreFloat3(&deltaPos2, deltaPos2Vec);

				XMStoreFloat3(&deltaUV1, deltaUV1Vec);
				XMStoreFloat3(&deltaUV1, deltaUV2Vec);

				float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
				XMVECTOR tangentVec = (deltaPos1Vec * deltaUV2.y - deltaPos2Vec * deltaUV1.y) * r;
				XMVECTOR bitangentVec = (deltaPos2Vec * deltaUV1.x - deltaPos1Vec * deltaUV2.x) * r;

				XMFLOAT3 tangent;
				XMFLOAT3 bitangent;

				XMStoreFloat3(&tangent, tangentVec);
				XMStoreFloat3(&bitangent, bitangentVec);

				meshData.Vertices.at(meshData.Indices.at(k)).Tangent = tangent;
				meshData.Vertices.at(meshData.Indices.at(k + 1)).Tangent = tangent;
				meshData.Vertices.at(meshData.Indices.at(k + 2)).Tangent = tangent;
				meshData.Vertices.at(meshData.Indices.at(k)).BiNormal = bitangent;
				meshData.Vertices.at(meshData.Indices.at(k + 1)).BiNormal = bitangent;
				meshData.Vertices.at(meshData.Indices.at(k + 2)).BiNormal = bitangent;

				k++;
			}
		}

		//Tangent and BiNormals
		if (aiMesh->HasTangentsAndBitangents())
		{
			for (UINT i = 0; i < aiMesh->mNumVertices; i++)
			{
				meshData.Vertices.at(i).Tangent = XMFLOAT3(reinterpret_cast<const float*>(&aiMesh->mTangents[i]));
				meshData.Vertices.at(i).BiNormal = XMFLOAT3(reinterpret_cast<const float*>(&aiMesh->mBitangents[i]));
			}
		}

		// Texture Coordinates
		UINT uvChannelCount = aiMesh->GetNumUVChannels();
		for (UINT i = 0; i < uvChannelCount; i++)
		{
			aiVector3D* aiTextureCoordinates = aiMesh->mTextureCoords[i];
			for (UINT j = 0; j < aiMesh->mNumVertices; j++)
			{
				meshData.Vertices[j].Texture = XMFLOAT2(reinterpret_cast<const float*>(&aiTextureCoordinates[j]));
			}

			break;
		}

		//Vertex colors
		UINT colorChannelCount = aiMesh->GetNumColorChannels();

		if (colorChannelCount > 0)
		{
			meshData.HasColor = true;

			for (UINT i = 0; i < colorChannelCount; i++)
			{
				aiColor4D* aiVertexColors = aiMesh->mColors[i];

				for (UINT j = 0; j < aiMesh->mNumVertices; j++)
				{
					meshData.Vertices.at(j).Color = XMFLOAT4(reinterpret_cast<const float*>(&aiVertexColors[j]));
				}
			}
		}
		else
		{
			for (UINT j = 0; j < aiMesh->mNumVertices; j++)
			{
				meshData.Vertices.at(j).Color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
			}
		}
	}
}