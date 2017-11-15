#pragma once

#include <DirectXComponentsPCH.h>
#include "Texture.h"

using namespace DirectX;

namespace Fab
{
	class TextureManager
	{
	public:
		TextureManager();
		~TextureManager();
		void       Initialise();
		void       Load(WCHAR* fileName, std::string name);
		void       InsertTexture(std::string name, TexturePtr shader);
		Texture&   GetTexture(std::string name);
		TexturePtr GetTexturePtr(std::string name);

	private:
		std::map<std::string, TexturePtr> _textures;
	};
}