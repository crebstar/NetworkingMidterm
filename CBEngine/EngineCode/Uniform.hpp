#ifndef included_Uniform
#define included_Uniform
#pragma once

#include "Vector3D.hpp"
#include "Matrix44.hpp"

typedef enum {

	TYPE_UNKNOWN,
	TYPE_INT,
	TYPE_FLOAT,
	TYPE_VECTOR3,
	TYPE_MATRIX44,
	TYPE_MATRIX44_ARRAY,

} Uniform_Type;

const size_t NUM_BONE_MATRIX_PASSED_TO_SHADER = 200;

class Uniform {
public:
	~Uniform() {}
	Uniform() {
		m_uniformLocation = -1;
		m_uniformType = TYPE_UNKNOWN;
	}

	Uniform_Type		m_uniformType;
	int					m_uniformLocation;

	// Possible Types ( Really don't like this but my template plan fell apart )
	int								m_uniformInt;
	float							m_uniformFloat;
	cbengine::Vector3<float>		m_uniformVector;
	Matrix44<float>					m_uniformMatrix44;
	std::vector<Matrix44<float>>    m_uniformMatrix44Vector;

};




#endif