
#ifndef included_Particle
#define included_Particle
#pragma once

/* 
	Base class for all objects in the game which have kinematical properties
*/

#include "EngineMacros.hpp"

#include "Vector3D.hpp"

class Particle {
public:

	virtual ~Particle();
	Particle();

	virtual void update( float deltaSeconds ) = 0;
	virtual void render( float deltaSeconds ) const = 0;

	cbengine::Vector3<float> m_position;
	cbengine::Vector3<float> m_velocity;

protected:


private:
	PREVENT_COPY_AND_ASSIGN( Particle );

};

#endif