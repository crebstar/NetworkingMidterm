




#ifndef included_Size
#define included_Size
#pragma once

#include "EngineMacros.hpp"

namespace cbengine {

class Size {

public:

	virtual ~Size();

	explicit Size( float width = 0.0f, float height = 0.0f );
	
	// TODO :: Define operators

	bool sizeEqualsSize( const Size & sizeToCompare ) const;

	// Public Instance Variables
	float		 m_width;
	float		 m_height;

protected:

private:

};

} // end namespace

#endif