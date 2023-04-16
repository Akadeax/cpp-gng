#include "base.h"
#include "TextureCache.h"

#include <iostream>
#include "Texture.h"

TextureCache::TextureCache(std::string TextureFolder)
	: m_TextureDirectory{ TextureFolder }
{
}

TextureCache::~TextureCache()
{
	UnloadAllTextures();
}

Texture* TextureCache::LoadTexture(const std::string& name, const std::string& resourceLocation)
{
	if (m_Textures.count(name) == 1)
	{
		std::cerr << "A texture of name '" << name << "' is already in cache";
		return nullptr;
	}

	Texture* tex{ new Texture(m_TextureDirectory + resourceLocation) };
	m_Textures[name] = tex;
    printf("Loaded Texture: %s %s\n", m_TextureDirectory.c_str(), resourceLocation.c_str());
	return tex;
}

Texture* TextureCache::GetTexture(const std::string& name)
{
	if (m_Textures.count(name) == 0)
	{
		std::cerr << "A texture of name '" << name << "' could not be found in cache";
		return nullptr;
	}

	return m_Textures[name];
}

void TextureCache::UnloadAllTextures()
{
	for (const std::pair< std::string, Texture*> pair : m_Textures)
	{
		delete pair.second;
	}
	m_Textures.clear();
}

void TextureCache::SetTextureDirectory(std::string folder)
{
    this->m_TextureDirectory = folder;
}
