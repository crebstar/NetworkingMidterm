
#ifndef included_Vector4
#define included_Vector4
#pragma once

namespace cbengine {

template <typename T>
class Vector4 {

public:
	~Vector4<T>() {

	} // end dtor

	Vector4<T>() {

		x = (T) 0.0f;
		y = (T) 0.0f;
		z = (T) 0.0f;
		w = (T) 0.0f;

	} // end ctor

	explicit Vector4<T>( T xComp, T yComp, T zComp, T wComp ) {

		x = xComp;
		y = yComp;
		z = zComp;
		w = wComp;
		
	} // end ctor

	T x;
	T y;
	T z;
	T w;

protected:

private:


};


} // end namespace

#endif