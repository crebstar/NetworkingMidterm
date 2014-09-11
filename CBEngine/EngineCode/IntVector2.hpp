
#ifndef included_IntVector
#define included_IntVector
#pragma once

namespace cbengine {

class IntVector2 {

public:

	~IntVector2() {

	} // end dtor

	IntVector2() {

		x = 0;
		y = 0;

	} // end ctor

	explicit IntVector2( int xComp, int yComp ) {

		x = xComp;
		y = yComp;

	} // end ctor


	bool operator < ( const IntVector2 & rhs ) const {

		if ( y < rhs.y ) {

			return true;

		} else if ( y > rhs.y ) {

			return false;

		} else {
			// The x is the tie breaker
			if ( x < rhs.x ) {

				return true;

			} else {

				return false;

			} // end inner if 

		} // end if


	} // end operator

	// Data Members
	int x;
	int y;

protected:


private:


};

} // end namespace

#endif