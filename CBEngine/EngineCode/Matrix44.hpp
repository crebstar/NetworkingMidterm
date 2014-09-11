
#ifndef included_Matrix44
#define included_Matrix44
#pragma once

#include <utility>
#include <math.h>

#include "Vector3D.hpp"

const size_t MATRIX44_NUM_ROWS = 4;
const size_t MATRIX44_NUM_COLS = 4;
const size_t MATRIX44_SIZE	   = MATRIX44_NUM_ROWS * MATRIX44_NUM_COLS;

// ROW MAJOR
// [   0 1 2 3    ]
// [   4 5 6 7    ]
// [   8 9 10 11  ]
// [  12 13 14 15 ]

// COL MAJOR
// [ 0 4 8  12 ]
// [ 1 5 9  13 ]
// [ 2 6 10 14 ]
// [ 3 7 11 15 ]

template < typename T >
class Matrix44 {

public:

	~Matrix44<T>() {}

	// Static functions
	// Factory Methods

		inline static Matrix44<float> createIdentityMatrixFloat() {

		Matrix44<T> identityMatrix;
		// Col One
		identityMatrix.matrixData[0] = 1.0f;
		identityMatrix.matrixData[1] = 0.0f;
		identityMatrix.matrixData[2] = 0.0f;
		identityMatrix.matrixData[3] = 0.0f;
		// Col Two
		identityMatrix.matrixData[4] = 0.0f;
		identityMatrix.matrixData[5] = 1.0f;
		identityMatrix.matrixData[6] = 0.0f;
		identityMatrix.matrixData[7] = 0.0f;
		// Col Three
		identityMatrix.matrixData[8] = 0.0f;
		identityMatrix.matrixData[9] = 0.0f;
		identityMatrix.matrixData[10] = 1.0f;
		identityMatrix.matrixData[11] = 0.0f;
		// Col Four
		identityMatrix.matrixData[12] = 0.0f;
		identityMatrix.matrixData[13] = 0.0f;
		identityMatrix.matrixData[14] = 0.0f;
		identityMatrix.matrixData[15] = 1.0f;

		return identityMatrix;

	}

	inline static Matrix44<float>  createTranslateMatrixFloat( const cbengine::Vector3<float> & positionOfObject ) {
		// Row Major Format And Assumes Transfer Of Ownership

		Matrix44<T> transformMatrix;
		// Row One
		transformMatrix.matrixData[ 0 ] = 1.0f;
		transformMatrix.matrixData[ 1 ] = 0.0f;
		transformMatrix.matrixData[ 2 ] = 0.0f;
		transformMatrix.matrixData[ 3 ] = 0.0f;
		// Row Two
		transformMatrix.matrixData[ 4 ] = 0.0f;
		transformMatrix.matrixData[ 5 ] = 1.0f;
		transformMatrix.matrixData[ 6 ] = 0.0f;
		transformMatrix.matrixData[ 7 ] = 0.0f;
		// Row Three
		transformMatrix.matrixData[ 8 ] = 0.0f;
		transformMatrix.matrixData[ 9 ] = 0.0f;
		transformMatrix.matrixData[ 10 ] = 1.0f;
		transformMatrix.matrixData[ 11 ] = 0.0f;
		//Row Four
		transformMatrix.matrixData[ 12 ] = positionOfObject.x;
		transformMatrix.matrixData[ 13 ] = positionOfObject.y;
		transformMatrix.matrixData[ 14 ] = positionOfObject.z;
		transformMatrix.matrixData[ 15 ] = 1.0f;

		return transformMatrix;

	}

	inline static Matrix44<double> createTransformMatrixDouble( const cbengine::Vector3<double> & positionOfObject ) {
		// Row Major Format And Assumes Transfer Of Ownership

		Matrix44<T>  transformMatrix;
		// Row One
		transformMatrix.matrixData[ 0 ] = 1.0;
		transformMatrix.matrixData[ 1 ] = 0.0;
		transformMatrix.matrixData[ 2 ] = 0.0;
		transformMatrix.matrixData[ 3 ] = 0.0;
		// Row Two
		transformMatrix.matrixData[ 4 ] = 0.0;
		transformMatrix.matrixData[ 5 ] = 1.0;
		transformMatrix.matrixData[ 6 ] = 0.0;
		transformMatrix.matrixData[ 7 ] = 0.0;
		// Row Three
		transformMatrix.matrixData[ 8 ] = 0.0;
		transformMatrix.matrixData[ 9 ] = 0.0;
		transformMatrix.matrixData[ 10 ] = 1.0;
		transformMatrix.matrixData[ 11 ] = 0.0;
		//Row Four
		transformMatrix.matrixData[ 12 ] = positionOfObject.x;
		transformMatrix.matrixData[ 13 ] = positionOfObject.y;
		transformMatrix.matrixData[ 14 ] = positionOfObject.z;
		transformMatrix.matrixData[ 15 ] = 1.0;

		return transformMatrix;

	}

	inline static Matrix44<double> createCannonicalRotationAboutXMatrixDouble( double angleRadians ) {

		Matrix44<T> cannonicalRotationMatrixAboutX;

		// Row One
		cannonicalRotationMatrixAboutX.matrixData[0]	= 1.0;
		cannonicalRotationMatrixAboutX.matrixData[1]	= 0.0;
		cannonicalRotationMatrixAboutX.matrixData[2]	= 0.0;
		cannonicalRotationMatrixAboutX.matrixData[3]	= 0.0;
		// Row Two
		cannonicalRotationMatrixAboutX.matrixData[4]	= 0.0;
		cannonicalRotationMatrixAboutX.matrixData[5]	= cos( angleRadians );
		cannonicalRotationMatrixAboutX.matrixData[6]	= sin( angleRadians );
		cannonicalRotationMatrixAboutX.matrixData[7]	= 0.0;
		// Row Three
		cannonicalRotationMatrixAboutX.matrixData[8]	= 0.0;
		cannonicalRotationMatrixAboutX.matrixData[9]	= -sin( angleRadians );
		cannonicalRotationMatrixAboutX.matrixData[10]   = cos( angleRadians );
		cannonicalRotationMatrixAboutX.matrixData[11]	= 0.0;
		// Row Four
		cannonicalRotationMatrixAboutX.matrixData[12]	= 0.0;
		cannonicalRotationMatrixAboutX.matrixData[13]	= 0.0;
		cannonicalRotationMatrixAboutX.matrixData[14]	= 0.0;
		cannonicalRotationMatrixAboutX.matrixData[15]   - 1.0;

		return cannonicalRotationMatrixAboutX;

	}

	inline static Matrix44<float> createCannonicalRotationAboutXMatrixFloat( float angleRadians ) {

		Matrix44<T> cannonicalRotationMatrixAboutX;

		// Row One
		cannonicalRotationMatrixAboutX.matrixData[0]	= 1.0f;
		cannonicalRotationMatrixAboutX.matrixData[1]	= 0.0f;
		cannonicalRotationMatrixAboutX.matrixData[2]	= 0.0f;
		cannonicalRotationMatrixAboutX.matrixData[3]	= 0.0f;
		// Row Two
		cannonicalRotationMatrixAboutX.matrixData[4]	= 0.0f;
		cannonicalRotationMatrixAboutX.matrixData[5]	= cos( angleRadians );
		cannonicalRotationMatrixAboutX.matrixData[6]	= sin( angleRadians );
		cannonicalRotationMatrixAboutX.matrixData[7]	= 0.0f;
		// Row Three
		cannonicalRotationMatrixAboutX.matrixData[8]	= 0.0f;
		cannonicalRotationMatrixAboutX.matrixData[9]	= -sin( angleRadians );
		cannonicalRotationMatrixAboutX.matrixData[10]   = cos( angleRadians );
		cannonicalRotationMatrixAboutX.matrixData[11]	= 0.0f;
		// Row Four
		cannonicalRotationMatrixAboutX.matrixData[12]	= 0.0f;
		cannonicalRotationMatrixAboutX.matrixData[13]	= 0.0f;
		cannonicalRotationMatrixAboutX.matrixData[14]	= 0.0f;
		cannonicalRotationMatrixAboutX.matrixData[15]   = 1.0f;

		return cannonicalRotationMatrixAboutX;

	}

	inline static Matrix44<double> createCannonicalRotationAboutYMatrixDouble( float angleRadians ) {

		Matrix44<T> cannonicalRotationMatrixAboutY;

		// Row One
		cannonicalRotationMatrixAboutY.matrixData[0]	= cos( angleRadians );
		cannonicalRotationMatrixAboutY.matrixData[1]	= 0.0;
		cannonicalRotationMatrixAboutY.matrixData[2]	= -sin( angleRadians );
		cannonicalRotationMatrixAboutY.matrixData[3]	= 0.0;
		// Row Two
		cannonicalRotationMatrixAboutY.matrixData[4]	= 0.0;
		cannonicalRotationMatrixAboutY.matrixData[5]	= 1.0;
		cannonicalRotationMatrixAboutY.matrixData[6]	= 0.0;
		cannonicalRotationMatrixAboutY.matrixData[7]    = 0.0;
		// Row Three
		cannonicalRotationMatrixAboutY.matrixData[8]	= sin( angleRadians );
		cannonicalRotationMatrixAboutY.matrixData[9]	= 0.0;
		cannonicalRotationMatrixAboutY.matrixData[10]	= cos( angleRadians );
		cannonicalRotationMatrixAboutY.matrixData[11]	= 0.0;
		// Row Four
		cannonicalRotationMatrixAboutY.matrixData[12]	= 0.0;
		cannonicalRotationMatrixAboutY.matrixData[13]	= 0.0;
		cannonicalRotationMatrixAboutY.matrixData[14]	= 0.0;
		cannonicalRotationMatrixAboutY.matrixData[15]	= 1.0;

		return cannonicalRotationMatrixAboutY;

	}


	inline static Matrix44<float> createCannonicalRotationAboutYMatrixFloat( float angleRadians ) {

		Matrix44<T> cannonicalRotationMatrixAboutY;

		// Row One
		cannonicalRotationMatrixAboutY.matrixData[0]	= cos( angleRadians );
		cannonicalRotationMatrixAboutY.matrixData[1]	= 0.0f;
		cannonicalRotationMatrixAboutY.matrixData[2]	= -sin( angleRadians );
		cannonicalRotationMatrixAboutY.matrixData[3]	= 0.0f;
		// Row Two
		cannonicalRotationMatrixAboutY.matrixData[4]	= 0.0f;
		cannonicalRotationMatrixAboutY.matrixData[5]	= 1.0f;
		cannonicalRotationMatrixAboutY.matrixData[6]	= 0.0f;
		cannonicalRotationMatrixAboutY.matrixData[7]    = 0.0f;
		// Row Three
		cannonicalRotationMatrixAboutY.matrixData[8]	= sin( angleRadians );
		cannonicalRotationMatrixAboutY.matrixData[9]	= 0.0f;
		cannonicalRotationMatrixAboutY.matrixData[10]	= cos( angleRadians );
		cannonicalRotationMatrixAboutY.matrixData[11]	= 0.0f;
		// Row Four
		cannonicalRotationMatrixAboutY.matrixData[12]	= 0.0f;
		cannonicalRotationMatrixAboutY.matrixData[13]	= 0.0f;
		cannonicalRotationMatrixAboutY.matrixData[14]	= 0.0f;
		cannonicalRotationMatrixAboutY.matrixData[15]	= 1.0f;

		return cannonicalRotationMatrixAboutY;

	}


	inline static Matrix44<double> createCannonicalRotationAboutZMatrixDouble( float angleRadians ) {

		Matrix44<T> cannonicalRotationMatrixAboutZ;

		// Row One
		cannonicalRotationMatrixAboutZ.matrixData[0]	= cos( angleRadians );
		cannonicalRotationMatrixAboutZ.matrixData[1]	= sin( angleRadians );
		cannonicalRotationMatrixAboutZ.matrixData[2]	= 0.0;
		cannonicalRotationMatrixAboutZ.matrixData[3]	= 0.0;
		// Row Two
		cannonicalRotationMatrixAboutZ.matrixData[4]	= -sin( angleRadians );
		cannonicalRotationMatrixAboutZ.matrixData[5]	= cos( angleRadians );
		cannonicalRotationMatrixAboutZ.matrixData[6]	= 0.0;
		cannonicalRotationMatrixAboutZ.matrixData[7]	= 0.0;
		// Row Three
		cannonicalRotationMatrixAboutZ.matrixData[8]	= 0.0;
		cannonicalRotationMatrixAboutZ.matrixData[9]	= 0.0;
		cannonicalRotationMatrixAboutZ.matrixData[10]	= 1.0;
		cannonicalRotationMatrixAboutZ.matrixData[11]	= 0.0;
		// Row Four
		cannonicalRotationMatrixAboutZ.matrixData[12]	= 0.0;
		cannonicalRotationMatrixAboutZ.matrixData[13]	= 0.0;
		cannonicalRotationMatrixAboutZ.matrixData[14]	= 0.0;
		cannonicalRotationMatrixAboutZ.matrixData[15]	= 1.0;

		return cannonicalRotationMatrixAboutZ;

	}


	inline static Matrix44<float> createCannonicalRotationAboutZMatrixFloat( float angleRadians ) {

		Matrix44<T> cannonicalRotationMatrixAboutZ;

		// Row One
		cannonicalRotationMatrixAboutZ.matrixData[0]	= cos( angleRadians );
		cannonicalRotationMatrixAboutZ.matrixData[1]	= sin( angleRadians );
		cannonicalRotationMatrixAboutZ.matrixData[2]	= 0.0f;
		cannonicalRotationMatrixAboutZ.matrixData[3]	= 0.0f;
		// Row Two
		cannonicalRotationMatrixAboutZ.matrixData[4]	= -sin( angleRadians );
		cannonicalRotationMatrixAboutZ.matrixData[5]	= cos( angleRadians );
		cannonicalRotationMatrixAboutZ.matrixData[6]	= 0.0f;
		cannonicalRotationMatrixAboutZ.matrixData[7]	= 0.0f;
	// Row Three
		cannonicalRotationMatrixAboutZ.matrixData[8]	= 0.0f;
		cannonicalRotationMatrixAboutZ.matrixData[9]	= 0.0f;
		cannonicalRotationMatrixAboutZ.matrixData[10]	= 1.0f;
		cannonicalRotationMatrixAboutZ.matrixData[11]	= 0.0f;
		// Row Four
		cannonicalRotationMatrixAboutZ.matrixData[12]	= 0.0f;
		cannonicalRotationMatrixAboutZ.matrixData[13]	= 0.0f;
		cannonicalRotationMatrixAboutZ.matrixData[14]	= 0.0f;
		cannonicalRotationMatrixAboutZ.matrixData[15]	= 1.0f;

		return cannonicalRotationMatrixAboutZ;

	}


	//General Math Functions
	inline static void matrixMultiply( const Matrix44<T> & lhs, const Matrix44<T> & rhs, Matrix44<T> & result ) {

		result.matrixData[0] = lhs.matrixData[0]*rhs.matrixData[0] + lhs.matrixData[1]*rhs.matrixData[4] + lhs.matrixData[2]*rhs.matrixData[8] + lhs.matrixData[3]*rhs.matrixData[12];
		result.matrixData[1] = lhs.matrixData[0]*rhs.matrixData[1] + lhs.matrixData[1]*rhs.matrixData[5] + lhs.matrixData[2]*rhs.matrixData[9] + lhs.matrixData[3]*rhs.matrixData[13];
		result.matrixData[2] = lhs.matrixData[0]*rhs.matrixData[2] + lhs.matrixData[1]*rhs.matrixData[6] + lhs.matrixData[2]*rhs.matrixData[10] + lhs.matrixData[3]*rhs.matrixData[14];
		result.matrixData[3] = lhs.matrixData[0]*rhs.matrixData[3] + lhs.matrixData[1]*rhs.matrixData[7] + lhs.matrixData[2]*rhs.matrixData[11] + lhs.matrixData[3]*rhs.matrixData[15];

		result.matrixData[4] = lhs.matrixData[4]*rhs.matrixData[0] + lhs.matrixData[5]*rhs.matrixData[4] + lhs.matrixData[6]*rhs.matrixData[8] + lhs.matrixData[7]*rhs.matrixData[12];
		result.matrixData[5] = lhs.matrixData[4]*rhs.matrixData[1] + lhs.matrixData[5]*rhs.matrixData[5] + lhs.matrixData[6]*rhs.matrixData[9] + lhs.matrixData[7]*rhs.matrixData[13];
		result.matrixData[6] = lhs.matrixData[4]*rhs.matrixData[2] + lhs.matrixData[5]*rhs.matrixData[6] + lhs.matrixData[6]*rhs.matrixData[10] + lhs.matrixData[7]*rhs.matrixData[14];
		result.matrixData[7] = lhs.matrixData[4]*rhs.matrixData[3] + lhs.matrixData[5]*rhs.matrixData[7] + lhs.matrixData[6]*rhs.matrixData[11] + lhs.matrixData[7]*rhs.matrixData[15];

		result.matrixData[8] = lhs.matrixData[8]*rhs.matrixData[0] + lhs.matrixData[9]*rhs.matrixData[4] + lhs.matrixData[10]*rhs.matrixData[8] + lhs.matrixData[11]*rhs.matrixData[12];
		result.matrixData[9] = lhs.matrixData[8]*rhs.matrixData[1] + lhs.matrixData[9]*rhs.matrixData[5] + lhs.matrixData[10]*rhs.matrixData[9] + lhs.matrixData[11]*rhs.matrixData[13];
		result.matrixData[10] = lhs.matrixData[8]*rhs.matrixData[2] + lhs.matrixData[9]*rhs.matrixData[6] + lhs.matrixData[10]*rhs.matrixData[10] + lhs.matrixData[11]*rhs.matrixData[14];
		result.matrixData[11] = lhs.matrixData[8]*rhs.matrixData[3] + lhs.matrixData[9]*rhs.matrixData[7] + lhs.matrixData[10]*rhs.matrixData[11] + lhs.matrixData[11]*rhs.matrixData[15];

		result.matrixData[12] = lhs.matrixData[12]*rhs.matrixData[0] + lhs.matrixData[13]*rhs.matrixData[4] + lhs.matrixData[14]*rhs.matrixData[8] + lhs.matrixData[15]*rhs.matrixData[12];
		result.matrixData[13] = lhs.matrixData[12]*rhs.matrixData[1] + lhs.matrixData[13]*rhs.matrixData[5] + lhs.matrixData[14]*rhs.matrixData[9] + lhs.matrixData[15]*rhs.matrixData[13];
		result.matrixData[14] = lhs.matrixData[12]*rhs.matrixData[2] + lhs.matrixData[13]*rhs.matrixData[6] + lhs.matrixData[14]*rhs.matrixData[10] + lhs.matrixData[15]*rhs.matrixData[14];
		result.matrixData[15] = lhs.matrixData[12]*rhs.matrixData[3] + lhs.matrixData[13]*rhs.matrixData[7] + lhs.matrixData[14]*rhs.matrixData[11] + lhs.matrixData[15]*rhs.matrixData[15];

	}
	
	// ********** Overloaded Operators ***********
	const T & operator []( int index ) {

		// No bounds checking. Needs to be fast imho
		return matrixData[index];

	}

	// ********** Inline Functions **************

	inline T getMatrixData( size_t row, size_t col ) {

		return matrixData[ row * MATRIX44_NUM_ROWS + MATRIX44_NUM_COLS ];

	}

	inline void transposeMatrix() {

		std::swap( matrixData[1], matrixData[4] );
		std::swap( matrixData[2], matrixData[8] );
		std::swap( matrixData[3], matrixData[12] );
		std::swap( matrixData[6], matrixData[9] );
		std::swap( matrixData[7], matrixData[13] );
		std::swap( matrixData[11], matrixData[14] );

	}

	// Matrix data
	T matrixData[ MATRIX44_SIZE ];

protected:



private:
	

};


#endif
