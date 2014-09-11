#ifndef included_Light
#define included_Light
#pragma once

#include "Vector3D.hpp"
#include "Vector4.hpp"

const float OUTER_DEFAULT_VALUE = 500000.0f;
const float INNER_DEFAULT_VALUE = OUTER_DEFAULT_VALUE - 1.0f;
const float DEFAULT_INNER_APPERTURE = -2.0f;
const float DEFAULT_OUTER_APPERTURE = -3.0f;
const cbengine::Vector3<float> DIRECTION_DEFAULT( 0.0f, 0.0f, 0.0f );

class Light {
public:
	explicit Light();
	~Light();

	cbengine::Vector3<float>	m_position;
	cbengine::Vector3<float>	m_direction;
	cbengine::Vector4<float>	m_colorAndBrightness;

	float						m_innerRadius;
	float						m_outerRadius;
	float						m_innerApertureDot;
	float						m_outerApertureDot;
	float						m_fractionAmbient;
	float						m_ignoresPosition;
	float						m_isLocalAmbient;

protected:

private:


};


#endif