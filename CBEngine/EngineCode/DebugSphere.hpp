
#ifndef included_DebugSphere
#define included_DebugSphere
#pragma once

#include "DebugShape.hpp"

#include "Vector4.hpp"

// void renderWireSphere(GLdouble radius, GLint slices, GLint stacks);
// void renderSolidSphere(GLdouble radius, GLint slices, GLint stacks);

const size_t NUMBER_SLICES = 20;
const size_t NUMBER_STACKS = 20;
const float SPHERE_LINE_WIDTH = 3.0f;

class DebugSphere : public DebugShape {

public:

	DebugSphere();
	virtual ~DebugSphere();

	virtual void update( float deltaSeconds );
	virtual void render( float deltaSeconds ) const;

	cbengine::Vector4<float> edgeColor;
	cbengine::Vector4<float> sideColor;
	double radius;

protected:


private:
	PREVENT_COPY_AND_ASSIGN( DebugSphere );


};

#endif