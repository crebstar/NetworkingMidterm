#include "Fog.hpp"


#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

Fog::~Fog() {

}


Fog::Fog() {
	// Ensure we don't pass garbage values by accident
	m_startDist = 0.0f;
	m_endDist = 0.0f;

	m_fogColorLocation = -1;
	m_startDistanceLocation = -1;
	m_endDistanceLocation = -1;

}


Fog::Fog( const cbengine::Vector4<float> & fogColor, float startDist, float endDist ) {

	m_fogColor = fogColor;
	m_startDist = startDist;
	m_endDist = endDist;

	m_fogColorLocation = -1;
	m_startDistanceLocation = -1;
	m_endDistanceLocation = -1;

}