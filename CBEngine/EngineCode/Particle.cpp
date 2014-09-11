
#include "Particle.hpp"

#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

Particle::~Particle() {

}

Particle::Particle() {

	m_position.x = 0.0f;
	m_position.y = 0.0f;
	m_position.z = 0.0f;

	m_velocity.x = 0.0f;
	m_velocity.y = 0.0f;
	m_velocity.z = 0.0f;

}