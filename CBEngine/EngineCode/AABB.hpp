

#ifndef included_AABB
#define included_AABB
#pragma once

#include "EngineMacros.hpp"
#include "Vector2.hpp"

namespace cbengine {

class AABB {

public:

	virtual ~AABB() {

	} // end Dtor

	// User is responsible for setting origin with this AABB ctor
	explicit AABB( float width = 0.0f, float height = 0.0f ) :
		m_width(width),
		m_height(height)
	{
		m_origin.x = 0.0f;
		m_origin.y = 0.0f;

	}

	explicit AABB( const Vector2 & origin, float width = 0.0f, float height = 0.0f ) :
		m_width(width),
		m_height(height)
	{
		
		m_origin.x = origin.x;
		m_origin.y = origin.y;

	} // end ctor

	// Instance Variables
	Vector2		m_origin;
	float		m_width;
	float		m_height;

protected:

private:

};

} // end namespace

#endif