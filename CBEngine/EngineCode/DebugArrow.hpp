
#ifndef included_DebugArrow
#define included_DebugArrow
#pragma once

#include "DebugShape.hpp"

#include "EngineMacros.hpp"

class DebugArrow : public DebugShape {

public:

	DebugArrow();
	virtual ~DebugArrow();

	virtual void update( float deltaSeconds );
	virtual void render( float deltaSeconds ) const;

	cbengine::Vector3<float>	startPosition;
	cbengine::Vector3<float>	endPosition;
	float						lengthOfArrow;

protected:


private:
	PREVENT_COPY_AND_ASSIGN( DebugArrow );

};

#endif