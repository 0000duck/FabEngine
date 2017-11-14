#include <DirectXComponentsPCH.h>
#include "VectorHelper.h"

namespace Fab
{
	const XMFLOAT3 Vector3Helper::Zero = XMFLOAT3(0.0f, 0.0f, 0.0f);
	const XMFLOAT3 Vector3Helper::One = XMFLOAT3(1.0f, 1.0f, 1.0f);
	const XMFLOAT3 Vector3Helper::Forward = XMFLOAT3(0.0f, 0.0f, -1.0f);
	const XMFLOAT3 Vector3Helper::Backward = XMFLOAT3(0.0f, 0.0f, 1.0f);
	const XMFLOAT3 Vector3Helper::Up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	const XMFLOAT3 Vector3Helper::Down = XMFLOAT3(0.0f, -1.0f, 0.0f);
	const XMFLOAT3 Vector3Helper::Right = XMFLOAT3(1.0f, 0.0f, 0.0f);
	const XMFLOAT3 Vector3Helper::Left = XMFLOAT3(-1.0f, 0.0f, 0.0f);
}