
#ifndef included_Sphere3D
#define included_Sphere3D
#pragma once

#include "Vector3D.hpp"

template < typename T >
class Sphere3D {

public:

	~Sphere3D<T>() {}
	Sphere3D<T>() {

		radius = 0.0;
		origin.x = 0.0;
		origin.y = 0.0;
		origin.z = 0.0;

	}

	explicit Sphere3D<T>( T radiusOfSphere, cbengine::Vector3<T> originOfSphere ) {

		radius = radiusOfSphere;
		origin.x = originOfSphere.x;
		origin.y = originOfSphere.y;
		origin.z = originOfSphere.z;

	}

	T radius;
	cbengine::Vector3<T> origin;

protected:


private:


};



#endif