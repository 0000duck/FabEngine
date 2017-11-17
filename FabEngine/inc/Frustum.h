#pragma once
#include <DirectXComponentsPCH.h>

using namespace DirectX;

namespace Fab
{
	class Frustum
	{
	public:
		Frustum();
		~Frustum();
		void BuildFrustum(XMFLOAT4X4 view, XMFLOAT4X4 projection, float farZ);
		bool CheckSphere(XMFLOAT4X4 view, XMFLOAT4X4 projection, XMFLOAT3 position, float radius);

	private:
		XMVECTOR _planes[6];
	};
}