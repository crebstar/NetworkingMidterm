
#ifndef included_AABB3D
#define included_AABB3D
#pragma once

#include "Vector3D.hpp"

namespace cbengine {

template< typename T >
class AABB3D {

public:

	~AABB3D() {} 

	AABB3D() {}

	AABB3D( const cbengine::Vector3<T> & minPointPosition, const cbengine::Vector3<T> & maxPointPosition ) {

		minPosition.x = minPointPosition.x;
		minPosition.y = minPointPosition.y;
		minPosition.z = minPointPosition.z;

		maxPosition.x = maxPointPosition.x;
		maxPosition.y = maxPointPosition.y;
		maxPosition.z = maxPointPosition.z;

	} // end ctor

	Vector3<T>		minPosition;
	Vector3<T>		maxPosition;

protected:



private:


};


} // end namespace

#endif