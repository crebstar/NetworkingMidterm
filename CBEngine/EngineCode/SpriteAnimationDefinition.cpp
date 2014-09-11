
#include "SpriteAnimationDefinition.hpp"

#include <cassert>
#include <math.h>

#include "SpriteAnimation.hpp"

#include "../EngineCode/TextureManager.hpp"


#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

using cbengine::SpriteAnimationDefinition;
using cbengine::SpriteAnimation;
using cbengine::Texture;
using cbengine::TextureManager;


SpriteAnimationDefinition::~SpriteAnimationDefinition() {



} // end Dtor

SpriteAnimationDefinition::SpriteAnimationDefinition( const std::string & animationTextureFileName, 
	int numRows, 
	int numColumns, 
	float durationSeconds ) 
{

	assert(numRows > 0);
	assert(numColumns > 0);
	assert(durationSeconds > 0.0f);

	this->loadAnimationTexture( animationTextureFileName );

	m_numberRowsInTexture = numRows;
	m_numberColumnsInTexture = numColumns;
	m_animationDuration = durationSeconds;
	m_animTimeStep = 0.0f;
	m_dirty = false;

	this->determineAnimationTimeStep();

} // end Ctor

SpriteAnimation * SpriteAnimationDefinition::createSpriteAnimationWithPosition( float xPos, float yPos) {

	SpriteAnimation * spriteAnim = new SpriteAnimation( this );
	spriteAnim->setPosition( xPos, yPos);
	return spriteAnim;

} 

bool SpriteAnimationDefinition::getTextureForAnimationOrExpire( float timeExisted, 
	cbengine::Texture * & animTexture, 
	cbengine::AABB & texturePosition ) 
{
	
	// Has it expired? If so, return true
	if (timeExisted >= m_animationDuration) return true;

	animTexture = m_animationTexture;

	float animationTexturePosition = floorf( timeExisted / m_animTimeStep );
	int rowPosition = static_cast<int>(floor(animationTexturePosition / static_cast<float>(m_numberRowsInTexture) ));
	int columnPosition = static_cast<int>(animationTexturePosition) - (rowPosition * m_numberColumnsInTexture);

	
	texturePosition.m_origin.x = static_cast<float>(columnPosition) * m_textureColumnStep;
	texturePosition.m_origin.y = static_cast<float>(rowPosition) * m_textureRowStep;
	texturePosition.m_width = m_textureColumnStep;
	texturePosition.m_height = m_textureRowStep;

	return false;

} // end getTextureForanimationOrExpire

void SpriteAnimationDefinition::loadAnimationTexture( const std::string & animationTextureFileName) {

	TextureManager * sharedTextureManager = TextureManager::sharedTextureManager();

	Texture * animTexture = sharedTextureManager->generateOrGrabExistingTexture( animationTextureFileName );

	assert(animTexture != nullptr);

	m_animationTexture = animTexture;

} // end loadAnimationTexture

void SpriteAnimationDefinition::determineAnimationTimeStep() {

	// Note: the + 1's are because of zero indexing

	m_numberOfAnimFrames = (EXPLOSION_NUM_ACTUAL_ROWS * (m_numberColumnsInTexture + 1) );

	m_animTimeStep =  ( m_animationDuration / static_cast<float>(m_numberOfAnimFrames) );

	m_textureRowStep = 1.0f / (m_numberRowsInTexture+ 1);
	m_textureColumnStep = 1.0f / (m_numberColumnsInTexture + 1);

} // end determineAnimationTimeStep