
#ifndef included_Geometry3D
#define included_Geometry3D
#pragma once

#include "Vector3D.hpp"

namespace cbengine {

void drawAxis( float lengthOfAxis, float lineWidth );
void drawVoxelCube( const Vector3<float> & cubeSideSizes );

} // end namespace

#endif