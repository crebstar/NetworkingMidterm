#ifndef included_Camera2D
#define included_Camera2D
#pragma once

#include "../../CBEngine/EngineCode/EngineMacros.hpp"
#include "../../CBEngine/EngineCode/Vector3D.hpp"
#include "../../CBEngine/EngineCode/Vector2.hpp"
#include "../../CBEngine/EngineCode/EulerAngles.hpp"
#include "../../CBEngine/EngineCode/Size.hpp"


#include "GameConstants.hpp"

//#include "GameObject.hpp"
//#include "ActionDelegate.hpp"

typedef cbengine::Size ViewSize;

class Camera2D {
public:
	~Camera2D();
	explicit Camera2D();

	// Core methods
	void update( float deltaSeconds );
	void updateNoClip( float deltaSeconds );
	void updateFollow( float deltaSeconds );
	void setUpCameraPositionForRendering() const;

	typedef enum {
		STATE_NOCLIP,
		STATE_FOLLOWING_GAME_OBJECT,
	} FollowState;

	cbengine::Vector2						m_position;
	cbengine::Vector2						m_velocity;
	cbengine::Vector2						m_center;
	ViewSize							    m_viewSize;
	FollowState								m_cameraState;

	// Inline Mutators
	//void setActionDelegate( ActionDelegate * actionDelegate );
	//void setGameObjectToFollow( GameObject* objectToFollow );
	void stopFollowingGameObject();

protected:

	void calculateHermiteSplineInterpolation( const cbengine::Vector2& startPosition,
		const cbengine::Vector2& desiredPosition,
		const cbengine::Vector2& cameraVelocity,
		const cbengine::Vector2& objectVelocity,
		cbengine::Vector2& out_position,
		cbengine::Vector2& out_velocity,
		float timeZeroToOne );

	float		m_currentT;
	float		m_splineSpeedCoefficient;
	float		m_objectDistanceThreshold;
	bool		m_objectToPositionThresholdCrossed;


private:
	void setCameraDefaults();
	void processMouseAndKeyboardInput( float deltaSeconds );
	void updateCameraCenterPosition();

	PREVENT_COPY_AND_ASSIGN( Camera2D )
};


// Inline functions
/*
inline void Camera2D::setActionDelegate( ActionDelegate * actionDelegate ) {

	if ( actionDelegate != nullptr ) {
		m_actionDelegate = actionDelegate;
	} // end if
} // end setter
*/

/*
inline void Camera2D::setGameObjectToFollow( GameObject* objectToFollow ) {

	if ( objectToFollow == nullptr ) {
		return;
	}
	m_objectToFollow = objectToFollow;
	const cbengine::Vector2& objectPosition = m_objectToFollow->getPosition();
	m_position.x = objectPosition.x - static_cast<float>( ( ladybug::ORTHO_SCREEN_WIDTH * 0.50f ) );
	m_position.y = objectPosition.y - static_cast<float>( ( ladybug::ORTHO_SCREEN_HEIGHT * 0.50f ) );
	m_cameraState = STATE_FOLLOWING_GAME_OBJECT;
}


inline void Camera2D::stopFollowingGameObject() {
	m_objectToFollow = nullptr;
	m_cameraState = STATE_NOCLIP;
}
*/

namespace CameraConstants {

const float CAMERA_VELOCITY_PER_SECOND = 20.0f;

const float CAMERA_ROTATION_DEGREES_PER_SECOND = 80.0f;
const float MOUSE_DAMPENING_FACTOR = 0.08f;

const float MAXIMUM_MOUSE_POSITION_DISPLACEMENT_AFTER_DAMPENING = 20.0f;

}


#endif