
#ifndef included_Vector2
#define included_Vector2
#pragma once

#include <string>

#include "EngineMacros.hpp"

namespace cbengine {

class Vector2 {

public:

	// Static methods
	static float dotProduct( const Vector2 & lhs, const Vector2 & rhs ) {

		float dotProductResult = ( lhs.x * rhs.x ) + ( lhs.y * rhs.y );
		return dotProductResult;

	}

	~Vector2();

	explicit Vector2( float x = 0.0f, float y = 0.0f);
	explicit Vector2( const std::string& vecString );

	void normalizeVector();

	float getMagnitudeOfVector() const;

	inline friend Vector2 operator * ( const Vector2& lhs, float scalar ) {

		Vector2 vectorToReturn;
		vectorToReturn.x = lhs.x * scalar;
		vectorToReturn.y = lhs.y * scalar;
		return vectorToReturn;
	}

	// TODO :: Define operators for vector operations
	Vector2 operator - ( const Vector2 & rhs ) {

		Vector2 vectorToReturn( this->x - rhs.x, this->y - rhs.y );
		return vectorToReturn;

	}

	Vector2 operator + ( const Vector2 & rhs ) {

		Vector2 vectorToReturn( this->x + rhs.x, this->y + rhs.y );
		return vectorToReturn;

	}

	// Multiplies each component of the vector by a scalar of the same type
	inline void operator *= ( float scalarToMultiply ) {

		x *= scalarToMultiply;
		y *= scalarToMultiply;

	}

	// Adds component of the other vector to current vector
	inline void operator += ( const Vector2 & rhs ) {
		x += rhs.x;
		y += rhs.y;
	} 

	// Subtracts component of the other vector to current vector
	inline void operator -= ( const Vector2 & rhs ) {
		x -= rhs.x;
		y -= rhs.y;
	} 

	inline bool areFloatsAlmostEqual( float x, float y ) const {
		const float EPSILON = 0.01f;
		if ( fabs( x - y ) <= EPSILON ) {
			return true;
		} else {
			return false;
		}
	}


	inline bool operator == ( const Vector2& rhs ) const {

		bool isAlmostEqual = areFloatsAlmostEqual( x, rhs.x );
		if ( isAlmostEqual ) {
			return areFloatsAlmostEqual( y, rhs.y );
		} else {
			return false;
		}
	}


	// Public Instance Variables
	float x;
	float y;
	
protected:


private:

};


inline float Vector2::getMagnitudeOfVector() const {

	float xSquared = x * x;
	float ySquared = y * y;
	return sqrt( ( xSquared + ySquared ) );
}

} // end namespace

#endif