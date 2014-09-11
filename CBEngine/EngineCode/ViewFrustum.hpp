
#ifndef included_ViewFrustum
#define included_ViewFrustum
#pragma once

#include "Plane3D.hpp"


class ViewFrustum {

public:

	~ViewFrustum();
	ViewFrustum();
	
	Plane3D<double> nearPlane;
	Plane3D<double> farPlane;
	Plane3D<double> rightPlane;
	Plane3D<double> leftPlane;
	Plane3D<double> topPlane;
	Plane3D<double> bottomPlane;

protected:


private:


};

#endif