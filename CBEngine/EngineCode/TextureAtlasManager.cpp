#include "TextureAtlasManager.hpp"

#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

TextureAtlasManager::~TextureAtlasManager() {
	std::map<std::string,TextureAtlas*>::iterator it;
	for ( it = m_textureAtlasMap.begin(); it != m_textureAtlasMap.end(); ++it ) {
		delete it->second;
	}
}