#include "Light.hpp"

#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

Light::~Light() {

}


Light::Light() {
	m_innerRadius = INNER_DEFAULT_VALUE;
	m_outerRadius = OUTER_DEFAULT_VALUE;
	m_innerApertureDot = DEFAULT_INNER_APPERTURE;
	m_outerApertureDot = DEFAULT_OUTER_APPERTURE;
	m_fractionAmbient = 0.0f;
	m_direction = DIRECTION_DEFAULT;
	m_ignoresPosition = 0.0f;
	m_isLocalAmbient = 0.0f;
}