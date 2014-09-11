
#ifndef included_DebugAABB
#define included_DebugAABB
#pragma once

#include "DebugShape.hpp"

#include "Vector3D.hpp"
#include "Vector4.hpp"

const float AABB_EDGE_LINE_WIDTH = 3.0f;

class DebugAABB : public DebugShape {

public:
	
	DebugAABB();
	virtual ~DebugAABB();

	virtual void update( float deltaSeconds );
	virtual void render( float deltaSeconds ) const;

	cbengine::Vector3<float> minPosition;
	cbengine::Vector3<float> maxPosition;

	cbengine::Vector4<float> edgeColor;
	cbengine::Vector4<float> sideColor;

protected:

	void renderDualModeImage() const;

private:
	PREVENT_COPY_AND_ASSIGN( DebugAABB );

};

#endif