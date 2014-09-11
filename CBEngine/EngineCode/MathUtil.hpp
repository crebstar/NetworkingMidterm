


#ifndef included_MathUtil
#define included_MathUtil
#pragma once

#include <Math.h>

#include "AABB.hpp"
#include "AABB3D.hpp"
#include "Disk2D.hpp"

namespace cbengine {

const float PI = 3.141592653589793f;
const float EPSILON = 0.01f;

inline float getRandomZeroToOne() {

	return static_cast<float>( rand() / static_cast<float>(RAND_MAX) );

}  // end getRandomZeroToOne

inline float radiansToDegrees( float radians ) {

	return radians * ( 180 / PI );

} // end radiansToDegrees

inline float degreesToRadians( float degrees ) {

	return degrees * ( PI / 180 );

} // end degreesToRadians

// Edge intersections will not flag here
inline bool doesAABB2IntersectAABB2( const AABB & boxOne, const AABB & boxTwo ) {


	return ( ( fabs(boxOne.m_origin.x - boxTwo.m_origin.x) * 2.0f ) < (boxOne.m_width + boxTwo.m_width) &&
		( fabs(boxOne.m_origin.y - boxTwo.m_origin.y) * 2.0f ) < (boxOne.m_height + boxTwo.m_height) );

} // end doesAABBIntersectAABB


inline bool doesAABB2IntersectOrTouchAABB2( const AABB & boxOne, const AABB & boxTwo ) {

	return ( ( fabs( boxOne.m_origin.x - boxTwo.m_origin.x) * 2.0f ) <= (boxOne.m_width + boxTwo.m_width) &&
		( fabs( boxOne.m_origin.y - boxTwo.m_origin.y) * 2.0f ) <= (boxOne.m_height + boxTwo.m_width) );

} // end doesAABBIntersectOrTouchAABB


inline bool doesAABB3IntersectAABB3( const AABB3D<float> & boxOne, const AABB3D<float> & boxTwo ) {

	return ( boxOne.maxPosition.x > boxTwo.minPosition.x &&
		boxOne.minPosition.x < boxTwo.maxPosition.x &&
		boxOne.maxPosition.y > boxTwo.minPosition.y &&
		boxOne.minPosition.y < boxTwo.maxPosition.y && 
		boxOne.maxPosition.z > boxTwo.minPosition.z && 
		boxOne.minPosition.z < boxTwo.maxPosition.z ); 

}


inline bool doesDiskIntersectDisk( const Disk2D & diskOne, const Disk2D & diskTwo ) {

	float xDist = diskOne.origin.x - diskTwo.origin.x;
	float yDist = diskOne.origin.y - diskTwo.origin.y;

	float distanceSquaredBetweenDisks = ( xDist * xDist ) + ( yDist * yDist );
	float radiusSumSquared = ( diskOne.radius + diskTwo.radius ) * ( diskOne.radius + diskTwo.radius );

	return ( radiusSumSquared > distanceSquaredBetweenDisks );

} // end doesDiskintersectDisk


inline bool doesDiskIntersectDiskOrTouch( const Disk2D & diskOne, const Disk2D & diskTwo ) {

	float xDist = diskOne.origin.x - diskTwo.origin.x;
	float yDist = diskOne.origin.y - diskTwo.origin.y;

	float distanceSquaredBetweenDisks = ( xDist * xDist ) + ( yDist * yDist );
	float radiusSumSquared = ( diskOne.radius + diskTwo.radius ) * ( diskOne.radius + diskTwo.radius );

	return ( radiusSumSquared >= distanceSquaredBetweenDisks );

} // end doesDiskintersectDisk


inline float rangeMapFloat( float inRangeStart, float inRangeEnd, float outRangeStart, float outRangeEnd, float inValue ) {

	// Handle the zero edge case
	if ( inRangeStart == inRangeEnd  ) {
		return 0.50f * ( outRangeStart + outRangeEnd );
	}

	float outValue = inValue;

	outValue = outValue - inRangeStart;
	outValue = outValue / ( inRangeEnd - inRangeStart );
	outValue = outValue * ( outRangeEnd - outRangeStart );
	outValue = outValue + outRangeStart;

	return outValue;

} // end rangeMapFloat


inline float roundFloat( float numToRound ) {

	return floorf( numToRound + 0.50f );
}


inline void clampFloat( float minFloatValue, float maxFloatValue, float & valueToClamp ) {

	if ( valueToClamp < minFloatValue ) {

		valueToClamp = minFloatValue;

	} else if ( valueToClamp > maxFloatValue ) {

		valueToClamp = maxFloatValue;
	} // end if
} // end function


inline void clampDouble( double minDoubleValue, double maxDoubleValue, double & valueToClamp ) {

	if ( valueToClamp < minDoubleValue ) {

		valueToClamp = minDoubleValue;

	} else if ( valueToClamp > maxDoubleValue ) {

		valueToClamp = maxDoubleValue;
	} // end if
} // end function


inline bool areFloatsAlmostEqual( float x, float y ) {

	if ( fabs( x - y ) <= EPSILON ) {
		return true;
	} else {
		return false;
	}

}

// Credit to the great Eiserloh
inline bool computeSurfaceTangentsAtVertex(
	Vector3<float> & surfaceTangentAtVertex_out,
	Vector3<float> & surfaceBitangentAtVertex_out,
	const Vector3<float> & normalAtThisVertex,
	const Vector3<float> & positionOfThisVertex,
	const Vector2 & texCoordsOfThisVertex,
	const Vector3<float> & positionOfPreviousAdjacentVertex,
	const Vector2 & texCoordsOfPreviousAdjacentVertex,
	const Vector3<float> & positionOfNextAdjacentVertex,
	const Vector2 & texCoordsOfNextAdjacentVertex )

{
	Vector3<float> vecToPrevious	= positionOfPreviousAdjacentVertex - positionOfThisVertex;
	Vector3<float> vecToNext		= positionOfNextAdjacentVertex - positionOfThisVertex;

	Vector2 texToPrevious;
	texToPrevious.x = texCoordsOfPreviousAdjacentVertex.x - texCoordsOfThisVertex.x;
	texToPrevious.y = texCoordsOfPreviousAdjacentVertex.y - texCoordsOfThisVertex.y;
	Vector2 texToNext;
	texToNext.x = texCoordsOfNextAdjacentVertex.x - texCoordsOfThisVertex.x;
	texToNext.y = texCoordsOfNextAdjacentVertex.y - texCoordsOfThisVertex.y;

	float invDeterminant = 1.0f / ((texToPrevious.x * texToNext.y) - (texToNext.x * texToPrevious.y));
	// (U02 * V01) - (U01 * V02)

	Vector3<float> uDirectionInWorldSpace(	
		(texToNext.y * vecToPrevious.x - texToPrevious.y * vecToNext.x),
		(texToNext.y * vecToPrevious.y - texToPrevious.y * vecToNext.y),
		(texToNext.y * vecToPrevious.z - texToPrevious.y * vecToNext.z)
		);

	Vector3<float> vDirectionInWorldSpace(
		(texToPrevious.x * vecToNext.x - texToNext.x * vecToPrevious.x),
		(texToPrevious.x * vecToNext.y - texToNext.x * vecToPrevious.y),
		(texToPrevious.x * vecToNext.z - texToNext.x * vecToPrevious.z)
		);

	uDirectionInWorldSpace *= invDeterminant;
	vDirectionInWorldSpace *= invDeterminant;

	if( abs( normalAtThisVertex.getLengthOfVector() - 1.f ) > 0.01f ) {
		// Error
		//int x = 3;
	}
		

	Vector3<float> & tangentFromCalculations = uDirectionInWorldSpace;
	Vector3<float> & bitangentFromCalculations = vDirectionInWorldSpace;
	Vector3<float> bitangentFromCross = Vector3<float>::cross( normalAtThisVertex, tangentFromCalculations );
	bitangentFromCross.normalizeVector();
	Vector3<float> tangentFromCross = Vector3<float>::cross( bitangentFromCross, normalAtThisVertex );

	float computedBitangentDotCrossBitangent = Vector3<float>::dot( bitangentFromCalculations, bitangentFromCross );

	bool isRightHanded = (computedBitangentDotCrossBitangent >= 0.f );
	if( !isRightHanded )
	{
		//bitangentFromCross *= -1.f;
	}

	surfaceTangentAtVertex_out = tangentFromCross;
	surfaceBitangentAtVertex_out = bitangentFromCross;

	return( isRightHanded );

}

/*
//-----------------------------------------------------------------------------------------------
// Returns TRUE if the Bitangent is "right handed" with respect to the Normal and Tangent;
// i.e. if [Tangent,Bitangent,Normal] is a right-handed basis.  Typically this bool is
// converted to a +/- 1.0 and passed into a vertex shader as the Tangent's "w" coordinate.
// The Bitangent is then computed within the vertex shader using cross product and "w" to
// disambiguate between which of the two possible perpendiculars to use for the Bitangent.
//
bool ComputeSurfaceTangentsAtVertex(
Vector3& surfaceTangentAtVertex_out,
Vector3& surfaceBitangentAtVertex_out,
const Vector3& normalAtThisVertex,
const Vector3& positionOfThisVertex,
const Vector2& texCoordsOfThisVertex,
const Vector3& positionOfPreviousAdjacentVertex,
const Vector2& texCoordsOfPreviousAdjacentVertex,
const Vector3& positionOfNextAdjacentVertex,
const Vector2& texCoordsOfNextAdjacentVertex )
{
Vector3 vecToPrevious	= positionOfPreviousAdjacentVertex - positionOfThisVertex;
Vector3 vecToNext		= positionOfNextAdjacentVertex - positionOfThisVertex;

Vector2 texToPrevious	= texCoordsOfPreviousAdjacentVertex - texCoordsOfThisVertex;
Vector2 texToNext		= texCoordsOfNextAdjacentVertex - texCoordsOfThisVertex;

float invDeterminant = 1.0f / ((texToPrevious.x * texToNext.y) - (texToNext.x * texToPrevious.y));
// (U02 * V01) - (U01 * V02)

Vector3 uDirectionInWorldSpace(	
(texToNext.y * vecToPrevious.x - texToPrevious.y * vecToNext.x),
(texToNext.y * vecToPrevious.y - texToPrevious.y * vecToNext.y),
(texToNext.y * vecToPrevious.z - texToPrevious.y * vecToNext.z)
);

Vector3 vDirectionInWorldSpace(
(texToPrevious.x * vecToNext.x - texToNext.x * vecToPrevious.x),
(texToPrevious.x * vecToNext.y - texToNext.x * vecToPrevious.y),
(texToPrevious.x * vecToNext.z - texToNext.x * vecToPrevious.z)
);

uDirectionInWorldSpace *= invDeterminant;
vDirectionInWorldSpace *= invDeterminant;

if( abs( normalAtThisVertex.CalcLength() - 1.f ) > 0.01f )
FatalError( "Bad normal", "bleagh." );

Vector3& tangentFromCalculations = uDirectionInWorldSpace;
Vector3& bitangentFromCalculations = vDirectionInWorldSpace;
Vector3 bitangentFromCross = CrossProduct( normalAtThisVertex, tangentFromCalculations );
bitangentFromCross.Normalize();
Vector3 tangentFromCross = CrossProduct( bitangentFromCross, normalAtThisVertex );

float computedBitangentDotCrossBitangent = DotProduct( bitangentFromCalculations, bitangentFromCross );
bool isRightHanded = (computedBitangentDotCrossBitangent >= 0.f );
if( !isRightHanded )
{
bitangentFromCross *= -1.f;
}

surfaceTangentAtVertex_out = tangentFromCross;
surfaceBitangentAtVertex_out = bitangentFromCross;

return( isRightHanded );
}


*/


} // end namespace


#endif