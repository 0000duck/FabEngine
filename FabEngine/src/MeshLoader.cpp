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

		//Tangent and BiNormals
		if (aiMesh->HasTangentsAndBitangents())
		{
			for (UINT i = 0; i < aiMesh->mNumVertices; i++)
			{
				meshData.Vertices.at(i).Tangent = XMFLOAT3(reinterpret_cast<const float*>(&aiMesh->mTangents[i]));
				meshData.Vertices.at(i).BiNormal = XMFLOAT3(reinterpret_cast<const float*>(&aiMesh->mBitangents[i]));
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
					k++;
				}
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