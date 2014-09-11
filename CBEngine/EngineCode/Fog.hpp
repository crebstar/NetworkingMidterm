#ifndef included_Fog
#define included_Fog
#pragma once

#include <string>

#include "Vector4.hpp"

// Note: This assumes shader conforms to these names
const std::string FOG_COLOR_UNIFORM_NAME = "u_fogColor";
const std::string FOG_START_DIST_NAME	 = "u_fogStartDist";
const std::string FOG_END_DIST_NAME		 = "u_fogEndDist";

class Fog {
public:

	~Fog();
	Fog();
	explicit Fog( const cbengine::Vector4<float> & fogColor, float startDist, float endDist );

	cbengine::Vector4<float>			m_fogColor;
	float								m_startDist;
	float								m_endDist;

	// Uniform locations
	int									m_fogColorLocation;
	int									m_startDistanceLocation;
	int									m_endDistanceLocation;

protected:

private:

};

#endif