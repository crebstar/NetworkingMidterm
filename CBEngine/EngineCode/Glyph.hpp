#ifndef included_Glyph
#define included_Glyph
#pragma once

#include "..\EngineCode\EngineMacros.hpp"

#include "Vector2.hpp"

class Glyph {

public:

	virtual ~Glyph();
	Glyph();

	// Inline Mutators
	//Setters
	void setUCSIndex( unsigned char ucsIndex );
	void setSheetNum( unsigned int sheetNum );
	void setMinTextureCoords( const cbengine::Vector2 & minTextureCoords );
	void setMaxTextureCoords( const cbengine::Vector2 & maxTextureCoords );
	void setTTFA( float ttfA );
	void setTTFB( float ttfB );
	void setTTFC( float ttfC );

	//Getters
	unsigned char getUCSIndex() const;
	unsigned int getSheetNum() const;
	const cbengine::Vector2 & getMinTextureCoords();
	const cbengine::Vector2 & getMaxTextureCoords();
	float getTTFA() const;
	float getTTFB() const;
	float getTTFC() const;

protected:

private:
	PREVENT_COPY_AND_ASSIGN( Glyph );

	unsigned char				m_ucsIndex;
	unsigned int				m_sheetNum;
	cbengine::Vector2			m_minTextureCoords;
	cbengine::Vector2			m_maxTextureCoords;
	float						m_ttfA;
	float						m_ttfB;
	float						m_ttfC;

};

//Inline Mutators
// Setters
inline void Glyph::setUCSIndex( unsigned char ucsIndex ) {
	m_ucsIndex = ucsIndex;
}


inline void Glyph::setSheetNum( unsigned int sheetNum ) {
	m_sheetNum = sheetNum;
}


inline void Glyph::setMinTextureCoords( const cbengine::Vector2 & minTextureCoords ) {
	m_minTextureCoords = minTextureCoords;
}


inline void Glyph::setMaxTextureCoords( const cbengine::Vector2 & maxTextureCoords ) {
	m_maxTextureCoords = maxTextureCoords;
}


inline void Glyph::setTTFA( float ttfA ) {
	m_ttfA = ttfA;
}


inline void Glyph::setTTFB( float ttfB ) {
	m_ttfB = ttfB;
}


inline void Glyph::setTTFC( float ttfC ) {
	m_ttfC = ttfC;
}

//Getters
inline unsigned char Glyph::getUCSIndex() const {
	return m_ucsIndex;
}


inline unsigned int Glyph::getSheetNum() const {
	return m_sheetNum;
}


inline const cbengine::Vector2 & Glyph::getMinTextureCoords() {
	return m_minTextureCoords;
}


inline const cbengine::Vector2 & Glyph::getMaxTextureCoords() {
	return m_maxTextureCoords;
}

inline float Glyph::getTTFA() const {
	return m_ttfA;
}


inline float Glyph::getTTFB() const {
	return m_ttfB;
}


inline float Glyph::getTTFC() const {
	return m_ttfC;
}

#endif