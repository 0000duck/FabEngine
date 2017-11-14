#pragma once

#include <DirectXComponentsPCH.h>

namespace Fab
{
	class Vector3Helper
	{
	public:
		static const XMFLOAT3 Zero;
		static const XMFLOAT3 One;
		static const XMFLOAT3 Forward;
		static const XMFLOAT3 Backward;
		static const XMFLOAT3 Up;
		static const XMFLOAT3 Down;
		static const XMFLOAT3 Right;
		static const XMFLOAT3 Left;

	private:
		Vector3Helper();
		Vector3Helper(const Vector3Helper& rhs) = delete;
		Vector3Helper& operator=(const Vector3Helper& rhs) = delete;
	};
}