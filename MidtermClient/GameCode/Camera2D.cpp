#include "Camera2D.hpp"

#include <math.h>

#include <windows.h>

#include "../../CBEngine/EngineCode/InputHandler.hpp"
#include "../../CBEngine/EngineCode/EngineCommon.hpp"
#include "../../CBEngine/EngineCode/MathUtil.hpp"
#include "../../CBEngine/EngineCode/Matrix44.hpp"
#include "../../CBEngine/EngineCode/MatrixStack.hpp"


Camera2D::~Camera2D() {
	// We shall be polite!
	ShowCursor( true );
} // end dtor


Camera2D::Camera2D() {

	setCameraDefaults();
	ShowCursor( true );
} // end ctor


void Camera2D::update( float deltaSeconds ) {

	if ( m_cameraState == STATE_NOCLIP ) {

		updateNoClip( deltaSeconds );

	} else if ( m_cameraState == STATE_FOLLOWING_GAME_OBJECT ) {

		updateFollow( deltaSeconds );
	}

	updateCameraCenterPosition();
} // end update


void Camera2D::updateFollow( float deltaSeconds ) {
	// TODO :: Add interpolation effects

	/*
	if ( m_objectToFollow->isFlaggedForDeletion() ) {

		m_objectToFollow = nullptr;
		m_cameraState = STATE_NOCLIP;
		return;
	}

	const cbengine::Vector2& objectPosition = m_objectToFollow->getPosition();
	const cbengine::Vector2& objectVelocity = m_objectToFollow->getCurrentVelocity();
	cbengine::Vector2 adjustedObjectPosition( ( objectPosition.x  - static_cast<float>( ( ladybug::ORTHO_SCREEN_WIDTH * 0.50f ) ) ),
		( objectPosition.y - static_cast<float>( ( ladybug::ORTHO_SCREEN_HEIGHT * 0.50f ) ) ) );
	float xPositionDif = fabsf( m_position.x - adjustedObjectPosition.x );
	float yPositionDif = fabsf( m_position.y - adjustedObjectPosition.y );
	float totalPosDif = xPositionDif + yPositionDif;

	if ( totalPosDif > m_objectDistanceThreshold ) {

		m_objectToPositionThresholdCrossed = true;
	}

	if ( m_objectToPositionThresholdCrossed ) {

		m_currentT += ( m_splineSpeedCoefficient * deltaSeconds );

		if ( m_currentT >= 1.0f ) {

			m_objectToPositionThresholdCrossed = false;
			m_currentT = 0.0f;

		} else {

			cbengine::Vector2 cameraPositionFromSpline;
			cbengine::Vector2 cameraVelocityFromSpline;
			calculateHermiteSplineInterpolation( m_position,
				adjustedObjectPosition,
				m_velocity,
				objectVelocity,
				cameraPositionFromSpline,
				cameraVelocityFromSpline,
				m_currentT );

			//m_position.x = cameraPositionFromSpline.x;
			//m_position.y = cameraPositionFromSpline.y;
			//m_velocity.x = cameraVelocityFromSpline.x;
			//m_velocity.y = cameraVelocityFromSpline.y;
			//m_position.x = m_position.x + ( m_velocity.x * deltaSeconds );
			//m_position.y = m_position.y + ( m_velocity.y * deltaSeconds );
		}

	}

	
	m_position.x = objectPosition.x - static_cast<float>( ( ladybug::ORTHO_SCREEN_WIDTH * 0.50f ) );
	m_position.y = objectPosition.y - static_cast<float>( ( ladybug::ORTHO_SCREEN_HEIGHT * 0.50f ) );
	//m_position.x = m_position.x + ( objectVelocity.x * deltaSeconds );
	//m_position.y = m_position.y + ( objectVelocity.y * deltaSeconds );

	m_center.x = m_position.x;
	m_center.y = m_position.y;

	*/
}


void Camera2D::updateNoClip( float deltaSeconds ) {

	processMouseAndKeyboardInput( deltaSeconds );
}


void Camera2D::updateCameraCenterPosition() {

	m_center.x = ( m_position.x + static_cast<float>( feedback::ORTHO_SCREEN_WIDTH * 0.50f ) );
	m_center.y = ( m_position.y + static_cast<float>( feedback::ORTHO_SCREEN_HEIGHT * 0.50f ) );
}


void Camera2D::setUpCameraPositionForRendering() const {

	MatrixStack * matrixStack = MatrixStack::sharedMatrixStack();
	cbengine::Vector3<float> negativeCameraPosition( -m_position.x, -m_position.y, 0.0f );
	matrixStack->applyTranslationAndPushToStack( negativeCameraPosition );

} // end render


void Camera2D::processMouseAndKeyboardInput( float deltaSeconds ) {

	using namespace CameraConstants;
	const float TILE_UNIT = 1.0f;
	cbengine::Vector2 cameraMoveVector( 0.0f, 0.0f );

	// Handle Mouse Input
	POINT mousePoint;
	if ( GetCursorPos( &mousePoint ) ) {
		// TODO::
	} // end if

	// Handle Keyboard Input
	InputHandler& sharedInputHandler = InputHandler::getSharedInputHandler();
	const bool* virtualKeys = sharedInputHandler.getKeyboardStatus();

	/*
	// LOCKED FOR NOW
	if ( virtualKeys[ 'W' ] ) {
		cameraMoveVector.y += TILE_UNIT;
	}

	if ( virtualKeys [ 'D' ] ) {
		cameraMoveVector.x += TILE_UNIT;
	}

	if ( virtualKeys[ 'S' ] ) {
		cameraMoveVector.y -= TILE_UNIT;
	}

	if ( virtualKeys[ 'A' ] ) {
		cameraMoveVector.x -= TILE_UNIT;	
	}
	
	if ( cameraMoveVector.x > 0.0f && cameraMoveVector.y > 0.0f ) {
		cameraMoveVector.normalizeVector();
	}
	*/

	cameraMoveVector *= ( CAMERA_VELOCITY_PER_SECOND * deltaSeconds );
	m_position += cameraMoveVector;

	//SetCursorPos( static_cast<int>( ladybug::HALF_SCREEN_WIDTH ), 
	//	static_cast<int>( ladybug::HALF_SCREEN_HEIGHT ) );

} // endProcessMouseandKeyboard


void Camera2D::setCameraDefaults() {

	m_position.x = static_cast<float>( ( feedback::ORTHO_SCREEN_WIDTH * 0.50f ) );
	m_position.y = static_cast<float>( ( feedback::ORTHO_SCREEN_HEIGHT * 0.50f ) );
	m_viewSize.m_width = static_cast<float>( feedback::ORTHO_SCREEN_WIDTH );
	m_viewSize.m_height = static_cast<float>( feedback::ORTHO_SCREEN_HEIGHT );
	m_cameraState = STATE_NOCLIP;

	m_currentT = 0.0f;
	m_objectDistanceThreshold = 2.0f;
	m_objectToPositionThresholdCrossed = false;
	m_splineSpeedCoefficient = 0.80f;

} // end setDefaults


