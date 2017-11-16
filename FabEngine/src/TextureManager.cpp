#include <DirectXComponentsPCH.h>
#include "TextureManager.h"

namespace Fab
{
	TextureManager::TextureManager()
	{
	}

	TextureManager::~TextureManager()
	{
	}

	void TextureManager::Initialise()
	{
	}

	void TextureManager::Load(WCHAR* fileName, std::string name, int slot)
	{
		{
			TexturePtr texture(new Texture(fileName, slot));
			texture->Load();
			InsertTexture(name, std::move(texture));
		}
	}

	void TextureManager::InsertTexture(std::string name, TexturePtr shader)
	{
		_textures.insert(std::pair<std::string, TexturePtr>(name, std::move(shader)));
	}

	Texture& TextureManager::GetTexture(std::string name)
	{
		auto found = _textures.find(name);
		assert(found != _textures.end());

		return *found->second;
	}

	TexturePtr TextureManager::GetTexturePtr(std::string name)
	{
		auto found = _textures.find(name);
		assert(found != _textures.end());

		return found->second;
	}
}