#include "Disk2D.hpp"

#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

cbengine::Disk2D::~Disk2D() {

} // end dtor

cbengine::Disk2D::Disk2D() {

	radius = 0.0f;
	origin.x = 0.0f;
	origin.y = 0.0f;
} // end default ctor

cbengine::Disk2D::Disk2D( float radiusOfDisk, float xOrigin, float yOrigin ) {

	radius = radiusOfDisk;
	origin.x = xOrigin;
	origin.y = yOrigin;
} // end ctor

cbengine::Disk2D::Disk2D(  float radiusOfDisk, const Vector3<float> & originOfDisk ) {

	radius = radiusOfDisk;
	origin.x = originOfDisk.x;
	origin.y = originOfDisk.y;
} // end ctor