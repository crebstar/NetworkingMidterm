

#ifndef included_SpriteAnimationDefinition
#define included_SpriteAnimationDefinition
#pragma once

#include <string>

#include "../EngineCode/EngineMacros.hpp"
#include "../EngineCode/Texture.hpp"
#include "../EngineCode/AABB.hpp"

// Flyweight implementation
namespace cbengine {

	// TODO remove explosion dependent stuff
const int EXPLOSION_ANIMATION_ROWS = 7;
const int EXPLOSION_ANIMATION_COLUMNS = 7;
const float EXPLOSION_ANIMATION_DURATION = 1.05f;
const int EXPLOSION_NUM_ACTUAL_ROWS = 5;

class SpriteAnimation;

class SpriteAnimationDefinition {

public:

	// Constructors and Destructors
	virtual ~SpriteAnimationDefinition();

	SpriteAnimationDefinition( const std::string & animationTextureFileName, 
		int numRows, 
		int numColumns, 
		float durationSeconds);

	// Returns true if the animation has expired
	virtual bool getTextureForAnimationOrExpire( float timeExisted, 
		cbengine::Texture * & animTexture, 
		cbengine::AABB & texturePosition
		);

	// Creation Methods (Assumes transfer of ownership) TODO :: learn shared_ptr!!
	SpriteAnimation * createSpriteAnimationWithPosition( float xPos, float yPos);

	// Mutators and Predicates
	virtual inline bool isDirty() const {
		return m_dirty;
	}

	PREVENT_COPY_AND_ASSIGN(SpriteAnimationDefinition);
protected:

private:

	void loadAnimationTexture( const std::string & animationTextureFileName );
	void determineAnimationTimeStep();

	// Instance Variables
	cbengine::Texture *		m_animationTexture;
	int						m_numberRowsInTexture;
	int						m_numberColumnsInTexture;
	int						m_numberOfAnimFrames;
	float					m_animationDuration;
	float					m_animTimeStep;
	bool					m_dirty;

	// Optimization Variables
	float					m_textureRowStep;
	float					m_textureColumnStep;

};

} // end namespace

#endif