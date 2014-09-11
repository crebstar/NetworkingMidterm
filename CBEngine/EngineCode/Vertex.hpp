
#ifndef included_Vertex
#define included_Vertex
#pragma once

#include "Vector2.hpp"
#include "Vector3D.hpp"
#include "Vector4.hpp"

namespace cbengine {

class Vertex {

public:

	~Vertex() {}
	Vertex() {} 

	Vector3<float>			vertexPosition;
	Vector4<float>			vertexColor;
	Vector2					vertexTextureCoords;
	Vector3<float>			vertexNormal;
	Vector3<float>			vertexTangent;
	Vector3<float>			vertexBitangent;
	Vector4<int>			vertexBoneIndexes;
	Vector4<float>			vertexBoneWeights;

	// Inline Functions
	void applyLightingCoefficientToRGBColor( float lightingCoefficient );

protected:


private:


};

// Inline Functions
inline void Vertex::applyLightingCoefficientToRGBColor( float lightingCoefficient ) {

	vertexColor.x = vertexColor.x * lightingCoefficient;
	vertexColor.y = vertexColor.y * lightingCoefficient;
	vertexColor.z = vertexColor.z * lightingCoefficient;

} // end function

} // end namespace

#endif