


#ifndef included_Texture
#define included_Texture
#pragma once

#include "Size.hpp"
#include "EngineMacros.hpp"

namespace cbengine {

class Texture {

public:

	virtual ~Texture();

	explicit Texture( unsigned int glTextureID );
	
	// Mutators
	virtual inline unsigned int getGLTextureID() const {
		return m_glTextureID;
	}

	virtual inline void setTextureSize( int width, int height ) {
		m_textureSize.m_width = static_cast<float>(width);
		m_textureSize.m_height = static_cast<float>(height);
	}

	virtual inline Size & getTextureSize() {
		return m_textureSize;
	}

	virtual inline bool isDirty() {
		return m_dirty;
	}

	PREVENT_COPY_AND_ASSIGN(Texture);
protected:

private:

	// Private Instance Vars
	unsigned int		m_glTextureID;
	Size				m_textureSize;
	mutable bool		m_dirty;

};


} // end namespace

#endif