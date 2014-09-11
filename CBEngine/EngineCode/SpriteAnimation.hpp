#ifndef included_SpriteAnimation
#define included_SpriteAnimation
#pragma once

#include "../EngineCode/Sprite.hpp"
#include "../EngineCode/EngineMacros.hpp"
#include "../EngineCode/AABB.hpp"

// TODO :: Remove explosion dependencies
namespace cbengine {

// INCOMING HARD CODED HAX
const float EXPLOSION_WIDTH = 1.75f;
const float EXPLOSION_HEIGHT = 1.75f;

class SpriteAnimationDefinition;

class SpriteAnimation : public cbengine::Sprite {

public:

	virtual ~SpriteAnimation();

	// Add the def class as a param
	explicit SpriteAnimation( SpriteAnimationDefinition * animDef );

	// Core Functions
	virtual void update( float deltaSeconds );
	virtual void render() const;

	PREVENT_COPY_AND_ASSIGN(SpriteAnimation);
protected:

private:

	// Instance Variables
	SpriteAnimationDefinition *		m_animationDefinition;
	mutable cbengine::AABB			m_currentTexturePosition; // Ugh this is hacky I know... but render is const declared in cbengine::Node  = - (
	float							m_timeExistedInSeconds;

};

} // end namespace

#endif