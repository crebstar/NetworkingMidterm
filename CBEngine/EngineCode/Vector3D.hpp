
#ifndef included_Vector3D
#define included_Vector3D
#pragma once

#include <math.h>
#include <string>
#include <cassert>

namespace cbengine {

template< typename T >
class Vector3 {

public:

	~Vector3() {

	}

	explicit Vector3( T xComponent, T yComponent, T zComponent ) {
		x = xComponent;
		y = yComponent;
		z = zComponent;
	}

	Vector3() {
		x = 0;
		y = 0;
		z = 0;
	}

	explicit Vector3( const std::string& vector3AsString ) {
		
		if ( vector3AsString.length() == 0 ) {
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
		} else {
			if ( vector3AsString[0] == '(' ) {
				constructVectorFromStringWithParenthesis( vector3AsString );
			} else {
				constructVectorFromStringNoParenthesis( vector3AsString );
			}
		}
		
	}

	// Static Functions

	inline static T dot( const Vector3 & lhs, const Vector3 & rhs ) {

		return ( (lhs.x * rhs.x) + (lhs.y * rhs.y) + (lhs.z * rhs.z)  );

	}

	inline static Vector3 cross( const Vector3 & lhs, const Vector3 & rhs ) {

		float xComp = (lhs.y * rhs.z) - (lhs.z * rhs.y);
		float yComp = -(lhs.x * rhs.z) + (lhs.z * rhs.x);
		float zComp = (lhs.x * rhs.y) - (lhs.y * rhs.x);

		return Vector3( xComp, yComp, zComp );

	}

	// Overloaded Operators
	inline friend bool operator == ( const Vector3 & lhs, const Vector3 & rhs ) {
		
		return ( lhs.x == rhs.x &&
				lhs.y == rhs.y && 
				lhs.z == rhs.z );
	
	}

	// Adds components of two vectors together to form a new vector
	inline friend Vector3 operator + ( const Vector3 & lhs, const Vector3 & rhs ) {

		return Vector3 ( lhs.x + rhs.x , lhs.y + rhs.y, lhs.z + rhs.z );

	}

	inline friend Vector3 operator - ( const Vector3 & lhs, const Vector3 & rhs ) {

		return Vector3 ( lhs.x - rhs.x , lhs.y - rhs.y, lhs.z - rhs.z );

	}

	// Adds component of the other vector to current vector
	inline void operator += ( const Vector3 & rhs ) {

		x += rhs.x;
		y += rhs.y;
		z += rhs.z;

	} 

	inline void operator -= ( const Vector3 & rhs ) {

		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;

	} 

	inline void operator += ( T numToAdd ) {

		x += numToAdd;
		y += numToAdd;
		z += numToAdd;

	} 

	// Multiplies each component of the vector by a scalar of the same type
	inline void operator *= ( T scalarToMultiply ) {

		x *= scalarToMultiply;
		y *= scalarToMultiply;
		z *= scalarToMultiply;

	}

	// Divides each component of the vector by a scalar of the same type
	// Division by zero fails silently
	inline void operator /= ( T scalarToDivideBy ) {

		// Prevent divide by 0
		if (scalarToDivideBy == 0) return;

		x /= scalarToDivideBy;
		y /= scalarToDivideBy;
		z /= scalarToDivideBy;

	}


	inline Vector3<T> operator * ( T scalarToMultiply ) const {

		Vector3<T> vectorToReturn;
		vectorToReturn.x = x * scalarToMultiply;
		vectorToReturn.y = y * scalarToMultiply;
		vectorToReturn.z = z * scalarToMultiply;
		
		return vectorToReturn;

	}

	inline bool operator < ( const Vector3<T> & rhs ) const {

		if ( x > rhs.x ) {
			return true;
		} else if ( x < rhs.x ) {
			return false;
		} else if ( y > rhs.y ) {
			return true;
		} else if ( y < rhs.y ) {
			return false;
		} else if ( z > rhs.z ) {
			return true;
		} else {
			return false;
		}

	}

	
	inline bool operator == ( const Vector3<T> & rhs ) {

		return ( ( x == rhs.x ) && ( y == rhs.y ) && ( z == rhs.z ) );

	}

	// Vector Operations

	// Vector Length and Normalization
	inline T getLengthOfVector() const {

		return sqrt( (x * x) + (y * y) + (z * z) );

	} // endGetLengthOfVector

	inline void normalizeVector() {

		T lengthOfVector = getLengthOfVector();

		// Avoid the case where the zero vector may crash the program
		if ( lengthOfVector <= 0.0f ) return;

		// Normalize the components
		x = (x / lengthOfVector);
		y = (y / lengthOfVector);
		z = (z / lengthOfVector);

	} // end normalizeVector

	// Dot Product and Cross Product

	inline T dot( const Vector3 & rhs ) {

		return ( (x * rhs.x) + (y * rhs.y) + (z * rhs.z) );

	}

	

	// Vector Components
	T x;
	T y;
	T z;

protected:


private:

	void constructVectorFromStringWithParenthesis( const std::string & vector3AsString ) {
		std::string numbersFromString;
		size_t currentComponentIndex = 0;
		for (size_t i = 1; i < vector3AsString.size(); ++i ) {
			if ( vector3AsString[i] == ',' ) {
				if ( currentComponentIndex == 0 ) {
					x = static_cast<T>( std::atof( numbersFromString.c_str() ) );
				} else if ( currentComponentIndex == 1 ) {
					y = static_cast<T>( std::atof( numbersFromString.c_str() ) );
				} 
				currentComponentIndex++;
				numbersFromString.clear();
			} else if ( vector3AsString[i] == ')' ) {
				z = static_cast<T>( std::atof( numbersFromString.c_str() ) );
				break;
			} else if ( currentComponentIndex > 2 ) {
				// At this point it is likely the number as a vec4...
				z = static_cast<T>( std::atof( numbersFromString.c_str() ) );
				break;
			} else {
				numbersFromString += vector3AsString[i];
			}
		} // end for
	}


	void constructVectorFromStringNoParenthesis( const std::string & vector3AsString ) {
		std::string numbersFromString;
		size_t currentComponentIndex = 0;
		for (size_t i = 1; i < vector3AsString.size(); ++i ) {
			if ( vector3AsString[i] == ',' ) {
				if ( currentComponentIndex == 0 ) {
					x = static_cast<T>( std::atof( numbersFromString.c_str() ) );
				} else if ( currentComponentIndex == 1 ) {
					y = static_cast<T>( std::atof( numbersFromString.c_str() ) );
				} 
				currentComponentIndex++;
				numbersFromString.clear();
			} else if ( currentComponentIndex > 2 ) {
				// At this point it is likely the number as a vec4...
				z = static_cast<T>( std::atof( numbersFromString.c_str() ) );
				break;
			} else {
				numbersFromString += vector3AsString[i];
			}
		} // end for

		if ( currentComponentIndex == 2 ) {
			z = static_cast<T>( std::atof( numbersFromString.c_str() ) );
		}
		
	} // end function

};



} // end namespace



#endif