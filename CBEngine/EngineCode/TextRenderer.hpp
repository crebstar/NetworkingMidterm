#ifndef included_TextRenderer
#define included_TextRenderer
#pragma once

#include <string>

#include "EngineMacros.hpp"
#include "Vector2.hpp"
#include "Vector4.hpp"

class BitmapFont;

const cbengine::Vector4<float> defaultTextColor( 1.0f, 1.0f, 1.0f, 1.0f );

class TextRenderer {
public:

	static TextRenderer * sharedTextRenderer() {
		static TextRenderer sharedTextRenderer;
		return &sharedTextRenderer;
	}

	~TextRenderer();

	void drawDebugText( const std::string & textToDraw, 
		const BitmapFont& font, 
		float cellHeight, 
		const cbengine::Vector2 & screenPosition, 
		const cbengine::Vector4<float> & textColorRGBA = defaultTextColor );

protected:

private:
	PREVENT_COPY_AND_ASSIGN( TextRenderer );

	TextRenderer() {}

};

#endif