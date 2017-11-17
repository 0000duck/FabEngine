#pragma once

#include <DirectXComponentsPCH.h>
#include "Fog.h"

namespace Fab
{
	Fog::Fog(XMFLOAT4 fogColor, float fogStart, float fogRange)
		: _renderSystem(D3D11RenderSystem::GetRenderSystem())
		, _fogColor(fogColor)
		, _fogStart(fogStart)
		, _fogRange(fogRange)
	{
		Initialise();
	}

	Fog::~Fog()
	{
	}

	void Fog::Initialise()
	{
		FrameConstantBuffer* pFrameConstantBufferUpdate = _renderSystem.GetFrameConstantBufferUpdate();
		ID3D11Buffer** pFrameConstantBuffer = _renderSystem.GetFrameConstantBuffer();
		ID3D11DeviceContext** pContext = _renderSystem.GetPImmediateContext();

		pFrameConstantBufferUpdate->FogEnabled = XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);
		pFrameConstantBufferUpdate->FogColor = _fogColor;
		pFrameConstantBufferUpdate->FogStart = XMFLOAT4(_fogStart, 0.0f, 0.0f, 0.0f);
		pFrameConstantBufferUpdate->FogRange = XMFLOAT4(_fogRange, 0.0f, 0.0f, 0.0f);
		(*pContext)->UpdateSubresource(*pFrameConstantBuffer, 0, nullptr, pFrameConstantBufferUpdate, 0, 0);
	}

	void Fog::Update(float deltaTime, float totalTime)
	{
	}

	void Fog::Draw()
	{
	}
}