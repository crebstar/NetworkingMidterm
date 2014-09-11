
#ifndef included_DebugPoint
#define included_DebugPoint
#pragma once

#include "DebugShape.hpp"

class DebugPoint : public DebugShape {

public:

	DebugPoint();
	virtual ~DebugPoint();

	virtual void update( float deltaSeconds );
	virtual void render( float deltaSeconds ) const;

	float m_pointLineLength;

protected:


private:
	


};


#endif