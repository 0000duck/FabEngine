#pragma once

#include <DirectXComponentsPCH.h>

namespace Fab
{
	class Texture
	{
	public:
		Texture(WCHAR* szFileName);
		~Texture();
		void                       Load();
		void                       Use();
		ID3D11ShaderResourceView** GetResource();

	private:
		D3D11RenderSystem&        _renderSystem;
		WCHAR*                    _szFileName;
		ID3D11ShaderResourceView* _pTextureRV;
	};

	typedef std::shared_ptr<Texture> TexturePtr;
}