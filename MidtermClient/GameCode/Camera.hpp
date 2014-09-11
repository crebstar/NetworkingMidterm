
#ifndef included_Camera
#define included_Camera
#pragma once

#include "../../CBEngine/EngineCode/EngineMacros.hpp"

#include "../../CBEngine/EngineCode/Vector3D.hpp"
#include "../../CBEngine/EngineCode/EulerAngles.hpp"


const float CAMERA_VELOCITY_PER_SECOND = 48.0f;

const float CAMERA_ROTATION_DEGREES_PER_SECOND = 80.0f;
const float MOUSE_DAMPENING_FACTOR = 0.08f;

const float MAXIMUM_MOUSE_POSITION_DISPLACEMENT_AFTER_DAMPENING = 20.0f;

class Camera {

public:
	~Camera();
	explicit Camera();

	// Core methods
	void update( float deltaSeconds );
	void updateNoClip( float deltaSeconds );
	void setUpCameraPositionForRendering() const;

	EulerAngles										m_orientation;	
	cbengine::Vector3<float>						m_position;

protected:
	void processMouseAndKeyboardInput( float deltaSeconds );
	void setCameraDefaults();
	
private:
	PREVENT_COPY_AND_ASSIGN( Camera )

};


#endif