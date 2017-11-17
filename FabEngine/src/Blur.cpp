#pragma once

#include <DirectXComponentsPCH.h>
#include "Blur.h"

namespace Fab
{
	Blur::Blur()
		: _renderSystem(D3D11RenderSystem::GetRenderSystem())
	{
	}

	Blur::~Blur()
	{
	}

	void Blur::Initialise()
	{
		FrameConstantBuffer* pFrameConstantBufferUpdate = _renderSystem.GetFrameConstantBufferUpdate();
		ID3D11Buffer** pFrameConstantBuffer = _renderSystem.GetFrameConstantBuffer();
		ID3D11DeviceContext** pContext = _renderSystem.GetPImmediateContext();

		pFrameConstantBufferUpdate->BlurEnabled = XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);
		(*pContext)->UpdateSubresource(*pFrameConstantBuffer, 0, nullptr, pFrameConstantBufferUpdate, 0, 0);
	}

	void Blur::Update(float deltaTime, float totalTime)
	{
	}

	void Blur::Draw()
	{
	}
}