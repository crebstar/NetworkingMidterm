#include "TextureAtlas.hpp"

#include <cassert>
#include <vector>

#include "XMLParser.hpp"
#include "XMLDocument.hpp"
#include "XMLNode.hpp"
#include "TextureManager.hpp"


#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

TextureAtlas::~TextureAtlas() {

} // end dtor


TextureAtlas::TextureAtlas( const std::string& textureAtlasFilePath, const std::string& textureAtlasFileName, bool loadTextureAtlas /* = true */ ) {
	assert( textureAtlasFilePath.length() > 0 );
	assert( textureAtlasFileName.length() > 0 );
	m_textureAtlasTexture = nullptr;
	m_textureAtlasLoaded = false;
	m_textureAtlasFilePath = textureAtlasFilePath;
	m_textureAtlasFileName = textureAtlasFileName;

	if ( loadTextureAtlas ) {
		loadTextureAtlasFromXMLFile();
	} else {
		m_textureAtlasLoaded = false;
	}
} // end ctor

// TODO :: Refactor this and replace all assertion statements with custom error messages
bool TextureAtlas::loadTextureAtlasFromXMLFile() {
	using namespace TextureAtlasConstants;
	if ( m_textureAtlasLoaded ) {
		return m_textureAtlasLoaded;
	}

	// Load XML Document
	XMLParser& xmlParser = XMLParser::getSharedXMLParser();
	xmlParser.loadXMLDocument( m_textureAtlasFilePath, m_textureAtlasFileName, true );

	// Extract High Level Atlas Data
	XMLNode textureAtlasNode = xmlParser.getActiveDocumentRootNode();
	m_heightWidth.m_width = xmlParser.getFloatXMLAttribute( textureAtlasNode, TEXTURE_ATLAS_TEXTURE_WIDTH_ATTRIB_NAME );
	m_heightWidth.m_height = xmlParser.getFloatXMLAttribute( textureAtlasNode, TEXTURE_ATLAS_TEXTURE_HEIGHT_ATTRIB_NAME );
	std::string textureImagePath = xmlParser.getStringXMLAttribute( textureAtlasNode, TEXTURE_ATLAS_TEXTURE_IMAGE_PATH_ATTRIB_NAME );
	cbengine::TextureManager * textureManager = cbengine::TextureManager::sharedTextureManager();
	m_textureAtlasTexture = textureManager->generateOrGrabExistingTexture( textureImagePath );
	std::vector<XMLNode> spriteNodes;
	textureAtlasNode.getAllChildrenOfNode( spriteNodes );

	// Validity Tests (Temp)
	assert( m_textureAtlasTexture );
	assert( m_heightWidth.m_width > 0.0f );
	assert( m_heightWidth.m_height > 0.0f );
	assert( !spriteNodes.empty() );

	// Extract Each Sprite Node's Data
	for ( size_t i = 0; i < spriteNodes.size(); ++i ) {
		XMLNode& spriteNode = spriteNodes[i];
		std::string imageName =  xmlParser.getStringXMLAttribute( spriteNode, TEXTURE_ATLAS_SPRITE_NAME_ATTRIB_NAME );
		float spriteImageWidth = xmlParser.getFloatXMLAttribute( spriteNode, TEXTURE_ATLAS_SPRITE_WIDTH_ATTRIB_NAME );
		float spriteImageHeight = xmlParser.getFloatXMLAttribute( spriteNode, TEXTURE_ATLAS_SPRITE_HEIGHT_ATTRIB_NAME );
		cbengine::Size spriteSize( spriteImageWidth, spriteImageHeight );
		float xTexCoord = xmlParser.getFloatXMLAttribute( spriteNode, TEXTURE_ATLAS_SPRITE_XCOORD_ATTRIB_NAME );
		float yTexCoord = xmlParser.getFloatXMLAttribute( spriteNode, TEXTURE_ATLAS_SPRITE_YCOORD_ATTRIB_NAME );
		float normalizedMinXTexCoord = ( xTexCoord/m_heightWidth.m_width );
		float normalizedMinYTexCoord = ( yTexCoord/m_heightWidth.m_height );
		float normalizedXMaxTexCoord = ( ( xTexCoord + spriteImageWidth ) / m_heightWidth.m_width );
		float normalizedYMaxTexCoord = ( ( yTexCoord + spriteImageHeight ) / m_heightWidth.m_height );
		cbengine::Vector2 spriteMinTexCoords( normalizedMinXTexCoord, normalizedMinYTexCoord );
		cbengine::Vector2 spriteMaxTexCoords( normalizedXMaxTexCoord, normalizedYMaxTexCoord );
	
		SpriteData spriteData( imageName, spriteMinTexCoords, spriteMaxTexCoords, spriteSize, true );
		m_spriteDataMap.insert( std::pair<std::string,SpriteData>( imageName, spriteData ) );

	} // end for

	m_textureAtlasLoaded = true;
	return m_textureAtlasLoaded;
}