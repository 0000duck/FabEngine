#pragma once

#include <DirectXComponentsPCH.h>
#include "Filter.h"

namespace Fab
{
	const XMMATRIX Filter::FilterNone = XMMATRIX({
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	});

	const XMMATRIX Filter::FilterGreyScale = XMMATRIX({
		0.299f, 0.299f, 0.299f, 0.0f,
		0.587f, 0.587f, 0.587f, 0.0f,
		0.144f, 0.144f, 0.144f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	});

	const XMMATRIX Filter::FilterSepia = XMMATRIX({
		0.393f, 0.359f, 0.272f, 0.0f,
		0.769f, 0.686f, 0.534f, 0.0f,
		0.189f, 0.168f, 0.131f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f
	});

	const XMMATRIX Filter::FilterInvert = XMMATRIX({
		-1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	});

	Filter::Filter(std::string type)
		: _renderSystem(D3D11RenderSystem::GetRenderSystem())
		, _type(type)
	{
		Initialise();
	}

	Filter::~Filter()
	{
	}

	void Filter::Initialise()
	{
		FrameConstantBuffer* pFrameConstantBufferUpdate = _renderSystem.GetFrameConstantBufferUpdate();
		ID3D11Buffer** pFrameConstantBuffer = _renderSystem.GetFrameConstantBuffer();
		ID3D11DeviceContext** pContext = _renderSystem.GetPImmediateContext();

		pFrameConstantBufferUpdate->FilterColorEnabled = XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);

		if (_type == "none")
		{
			//pFrameConstantBufferUpdate->FilterColor = FilterNone;
			pFrameConstantBufferUpdate->FilterColor = XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);
		}
		else if (_type == "grey-scale")
		{
			//pFrameConstantBufferUpdate->FilterColor = FilterGreyScale;
			pFrameConstantBufferUpdate->FilterColor = XMFLOAT4(2.0f, 0.0f, 0.0f, 0.0f);
		}
		else if (_type == "sepia")
		{
			//pFrameConstantBufferUpdate->FilterColor = FilterSepia;
			pFrameConstantBufferUpdate->FilterColor = XMFLOAT4(3.0f, 0.0f, 0.0f, 0.0f);
		}
		else if (_type == "invert")
		{
			//pFrameConstantBufferUpdate->FilterColor = FilterInvert;
			pFrameConstantBufferUpdate->FilterColor = XMFLOAT4(4.0f, 0.0f, 0.0f, 0.0f);
		}

		(*pContext)->UpdateSubresource(*pFrameConstantBuffer, 0, nullptr, pFrameConstantBufferUpdate, 0, 0);
	}

	void Filter::Update(float deltaTime, float totalTime)
	{
	}

	void Filter::Draw()
	{
	}
}