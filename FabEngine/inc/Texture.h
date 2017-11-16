#pragma once

#include <DirectXComponentsPCH.h>

namespace Fab
{
	class Texture
	{
	public:
		Texture(WCHAR* szFileName, int slot);
		~Texture();
		void                       Load();
		void                       Use();
		ID3D11ShaderResourceView** GetResource();

	private:
		D3D11RenderSystem&        _renderSystem;
		WCHAR*                    _szFileName;
		ID3D11ShaderResourceView* _pTextureRV;
		int                       _slot;
	};

	typedef std::shared_ptr<Texture> TexturePtr;
}