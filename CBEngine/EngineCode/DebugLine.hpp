
#ifndef included_DebugLine
#define included_DebugLine
#pragma once

#include "DebugShape.hpp"

#include "Vector3D.hpp"
#include "Vector4.hpp"

class DebugLine : public DebugShape {

public:

	DebugLine();
	virtual ~DebugLine();

	virtual void update( float deltaSeconds );
	virtual void render( float deltaSeconds ) const;

	cbengine::Vector3<float> startVertex;
	cbengine::Vector3<float> endVertex;

	cbengine::Vector4<float> startingVertexColor;
	cbengine::Vector4<float> endingVertexColor;

protected:


private:

};

#endif