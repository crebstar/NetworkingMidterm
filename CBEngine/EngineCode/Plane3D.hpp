#ifndef included_Plane3D
#define included_Plane3D
#pragma once

#include "Vector3D.hpp"

template < typename T >
class Plane3D {

public:

	~Plane3D<T>() {}

	Plane3D<T>() {
		distance = 0.0;
	}

	explicit Plane3D<T>( cbengine::Vector3<T> normalToPlane , T dist ) {

		planeNormal.x = normalToPlane.x;
		planeNormal.y = normalToPlane.y;
		planeNormal.z = normalToPlane.z;
		distance = dist;

	}

	cbengine::Vector3<T> planeNormal;
	T distance;

protected:


private:


};

#endif