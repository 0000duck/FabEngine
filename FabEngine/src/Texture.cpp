#include <DirectXComponentsPCH.h>
#include "Texture.h"

namespace Fab
{
	Texture::Texture(WCHAR* szFileName, int slot)
		: _renderSystem(D3D11RenderSystem::GetRenderSystem())
		, _szFileName(szFileName)
		, _pTextureRV(nullptr)
		, _slot(slot)
	{
		_pTextureRV = nullptr;
	}

	Texture::~Texture()
	{
		SafeReleaseCom(_pTextureRV);
	}

	void Texture::Load()
	{
		ID3D11Device** pd3dDevice = _renderSystem.GetPd3dDevice();
		ID3D11DeviceContext** pContext = _renderSystem.GetPImmediateContext();

		HRESULT hr = CreateDDSTextureFromFile(*pd3dDevice, _szFileName, nullptr, &_pTextureRV);
		(*pContext)->PSSetShaderResources(_slot, 1, &_pTextureRV);
	}

	void Texture::Use()
	{
		ID3D11DeviceContext** pContext = _renderSystem.GetPImmediateContext();
		(*pContext)->PSSetShaderResources(_slot, 1, &_pTextureRV);
	}

	ID3D11ShaderResourceView** Texture::GetResource()
	{
		return &_pTextureRV;
	}
}