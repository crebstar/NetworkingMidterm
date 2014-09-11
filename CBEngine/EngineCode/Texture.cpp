
#include "Texture.hpp"

#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

using cbengine::Texture;

Texture::~Texture() {


} // end dtor

Texture::Texture( unsigned int glTextureID ) :
	m_glTextureID(glTextureID)
{

}

