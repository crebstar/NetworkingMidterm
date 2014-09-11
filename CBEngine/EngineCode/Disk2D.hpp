
#ifndef included_Disk2D
#define included_Disk2D
#pragma once

#include "Vector3D.hpp"
#include "Vector2.hpp"

namespace cbengine {

	class Disk2D {
	public:
		~Disk2D();
		Disk2D();
		explicit Disk2D( float radiusOfDisk, float xOrigin, float yOrigin );
		explicit Disk2D( float radiusOfDisk, const Vector3<float> & originOfDisk );

		// Todo think of how to handle origin with addition
		inline Disk2D operator + ( const Disk2D & rhs ) {
			Disk2D diskAdditionResult;
			diskAdditionResult.radius = this->radius + rhs.radius;
			return diskAdditionResult;
		} 

		// Public Variables
		float				radius;
		Vector2				origin;

	protected:

	private:

	};

}


#endif