


#ifndef included_Sprite
#define included_Sprite
#pragma once

#include "Node.hpp"

#include <string>

#include "EngineMacros.hpp"

namespace cbengine {

class Texture;

class Sprite : public Node {

public:

	virtual ~Sprite();

	Sprite();

	explicit Sprite( const std::string & spriteTextureFileName );

	virtual void update( float deltaSeconds );
	virtual void render() const;

	virtual void initWithTexture( const std::string & textureFileName );

	// Mutators
	virtual inline void setTexture( Texture * textureForSprite ) {

		if (textureForSprite != nullptr) {
			m_texture = textureForSprite;
		}

	}

	virtual inline const Texture * getSpriteTexture() const {
		return m_texture;
	}


	PREVENT_COPY_AND_ASSIGN(Sprite);
protected:

	// Private Functions
	virtual void drawBasedOnGLTexture() const;

	// Private Instance Variables
	Texture * m_texture;

private:

	
};

} // end namespace

#endif