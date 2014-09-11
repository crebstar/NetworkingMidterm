
#include "TextureManager.hpp"

#include <iostream>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/gL.h>

#define STBI_HEADER_FILE_ONLY
#include "stb_image.c"

#include "Texture.hpp"

#include "OpenGLRenderer.hpp"


#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

using cbengine::TextureManager;
using cbengine::Texture;

// TextureManager is responsible for freeing texture data
TextureManager::~TextureManager() {

	std::map< std::string , Texture *>::iterator it;

	for (it = m_textureCache.begin(); it != m_textureCache.end(); it++) {

		Texture * textureToDelete = it->second;
		delete textureToDelete;
	} // end for

} // end dtor


Texture * TextureManager::generateOrGrabExistingTexture( const std::string & textureFileName ) {

	std::map< std::string , Texture * >::iterator it = m_textureCache.find( textureFileName );

	// Texture already exists... return it
	if ( it != m_textureCache.end() ) {
		return it->second;
	}

	// Texture doesn't exist... Generate one
	Texture * texture;
	texture = this->generateTextureDataFromFile( textureFileName );

	// TODO write a log macro
	if (!texture) {

		std::cout << "Error texture " << textureFileName << " failed to load" << std::endl;
		return nullptr;

	} // end if 

	return texture;

} // end generateOrGrabExistingTexture


Texture * TextureManager::generateTextureDataFromFile( const std::string & textureFileName ) {

	// Variables needed to initialize texture
	// TODO :: Convert this to a struct
	const char * fileNameOfTexture = textureFileName.c_str();
	unsigned char * textureData;
	int textureWidth;
	int textureHeight;
	int comp; // How many color components are apart of the image (numColorComponents)
	int reqComp = 0; // What components you hope to have in the image
	
	// textureWidth and textureHeight are Out variables
	textureData = stbi_load(fileNameOfTexture, &textureWidth, &textureHeight, &comp, reqComp);

	// Could not load texture
	// TODO :: incorporate error messages
	if (textureData == NULL) {
		return nullptr;
	}

	// Use watch window with , and # to see contents of unsigned char array
	
	// A lot of potential for IO error here...
	// TODO :: Add error handling code and log statements
	
	// allocate texture name
	glEnable(GL_TEXTURE_2D);

	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

	GLuint textureLocation;
	glGenTextures( 1, &textureLocation);

	glBindTexture( GL_TEXTURE_2D, textureLocation);

	// Set texture clamp vs. wrap (repeat)
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT ); // GL_CLAMP or GL_REPEAT
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT ); // GL_CLAMP or GL_REPEAT

	// Set magnification (texel > pixel) and minification (texel < pixel) filters
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST ); // one of: GL_NEAREST, GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_LINEAR
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR ); // one of: GL_NEAREST, GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_LINEAR
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 5 );
	
	GLenum bufferFormat = GL_RGBA; // the format our source pixel data is currently in; any of: GL_RGB, GL_RGBA, GL_LUMINANCE, GL_LUMINANCE_ALPHA, ...
	if( comp == 3 ) {
		bufferFormat = GL_RGB;
	}

	GLenum internalFormat = bufferFormat; // the format we want the texture to me on the card; allows us to translate into a different texture format as we upload to OpenGL

	glTexImage2D(GL_TEXTURE_2D, 
		0, 
		internalFormat, 
		textureWidth, 
		textureHeight, 
		0, 
		bufferFormat, 
		GL_UNSIGNED_BYTE, 
		textureData
	);

	glHint( GL_GENERATE_MIPMAP_HINT, GL_NICEST );
	glGenerateMipmap( GL_TEXTURE_2D ); // must get from glext

	Texture * texture = new Texture( textureLocation );
	texture->setTextureSize( textureWidth, textureHeight );

	this->cacheTexture( textureFileName , texture );

	stbi_image_free( textureData );

	return texture;

} // end generateTextureDataFromFile


void TextureManager::cacheTexture( const std::string & textureName, Texture * textureToCache ) {

	std::pair< std::map< std::string, Texture *>::iterator, bool> insertCheck;
	insertCheck = m_textureCache.insert( std::pair< std::string, Texture *>( std::string(textureName) , textureToCache) );

	if (insertCheck.second == false) {
		// Texture already cached
		// Put log statement here when we have a working logger
	} 

} // endCacheTexture