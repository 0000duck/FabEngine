#pragma once

#include <DirectXComponentsPCH.h>
#include "Shader.h"

using namespace DirectX;

namespace Fab
{
	struct VertexColor
	{
		XMFLOAT3 Position;
		XMFLOAT4 Color;
		XMFLOAT3 Normal;
		XMFLOAT3 Tangent;
		XMFLOAT3 BiNormal;

		VertexColor()
		{
		}

		VertexColor(XMFLOAT3 position, XMFLOAT4 color)
			: Position(position)
			, Color(color)
		{
		}

		VertexColor(XMFLOAT3 position, XMFLOAT4 color, XMFLOAT3 normal, XMFLOAT3 tangent, XMFLOAT3 biNormal)
			: Position(position)
			, Color(color)
			, Normal(normal)
			, Tangent(tangent)
			, BiNormal(biNormal)
		{
		}
	};
}