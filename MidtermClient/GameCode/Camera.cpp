
#include "Camera.hpp"

#include <math.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "../../CBEngine/EngineCode/EngineCommon.hpp"
#include "../../CBEngine/EngineCode/InputHandler.hpp"
#include "../../CBEngine/EngineCode/MathUtil.hpp"
#include "../../CBEngine/EngineCode/Matrix44.hpp"
#include "../../CBEngine/EngineCode/MatrixStack.hpp"

#include "GameConstants.hpp"

#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

Camera::~Camera() {

	// We shall be polite!
	ShowCursor( true );
} // end dtor


Camera::Camera() {

	setCameraDefaults();
	ShowCursor( false );
} // end ctor


void Camera::update( float deltaSeconds ) {

	updateNoClip( deltaSeconds );
} // end update

void Camera::updateNoClip( float deltaSeconds ) {

	processMouseAndKeyboardInput( deltaSeconds );
}


void Camera::setUpCameraPositionForRendering() const {
	
	MatrixStack * matrixStack = MatrixStack::sharedMatrixStack();
	// Roll, pitch, yaw
	float rollRadiansAboutX = cbengine::degreesToRadians( -m_orientation.rollDegreesAboutX );
	float pitchRadiansAboutY = cbengine::degreesToRadians( -m_orientation.pitchDegreesAboutY );
	float yawRadiansAboutZ = cbengine::degreesToRadians( -m_orientation.yawDegreesAboutZ );

	Matrix44<float> cameraViewMatrix;
	Matrix44<float> rotationAboutXAxis = Matrix44<float>::createCannonicalRotationAboutXMatrixFloat( rollRadiansAboutX );
	Matrix44<float> rotationAboutYAxis = Matrix44<float>::createCannonicalRotationAboutYMatrixFloat( pitchRadiansAboutY );
	Matrix44<float> rotationAboutZAxis = Matrix44<float>::createCannonicalRotationAboutZMatrixFloat( yawRadiansAboutZ );

	Matrix44<float> rotXMultRotY;
	Matrix44<float>::matrixMultiply( rotationAboutXAxis, rotationAboutYAxis, rotXMultRotY );
	Matrix44<float> rotZMultRotXY;
	Matrix44<float>::matrixMultiply( rotationAboutZAxis, rotXMultRotY, rotZMultRotXY );

	cbengine::Vector3<float> negativeCameraPosition( -m_position.x, -m_position.y, -m_position.z );
	Matrix44<float> cameraTranslation = Matrix44<float>::createTranslateMatrixFloat( negativeCameraPosition );

	Matrix44<float>::matrixMultiply( cameraTranslation, rotZMultRotXY, cameraViewMatrix );

	matrixStack->applyTransformAndPushToStack( cameraViewMatrix );

	matrixStack->setWorldToCameraSpaceTransformMatrix( cameraViewMatrix );

	// Invert to get camera space -> world space matrix
	Matrix44<float> cameraToWorldSpaceTransform = cameraViewMatrix;
	cameraToWorldSpaceTransform.transposeMatrix();
	cameraToWorldSpaceTransform.matrixData[12] = -cameraToWorldSpaceTransform.matrixData[12];
	cameraToWorldSpaceTransform.matrixData[13] = -cameraToWorldSpaceTransform.matrixData[13];
	cameraToWorldSpaceTransform.matrixData[14] = -cameraToWorldSpaceTransform.matrixData[14];
	
	matrixStack->setCameraToWorldSpaceTransformMatrix( cameraToWorldSpaceTransform );
} // end render



void Camera::processMouseAndKeyboardInput( float deltaSeconds ) {
	// Mouse then keyboard
	float yawRadians = cbengine::degreesToRadians( m_orientation.yawDegreesAboutZ );
	cbengine::Vector3<float> cameraForwardXY( cos( yawRadians ) , sin( yawRadians ), 0.0f );
	cbengine::Vector3<float> cameraLeftXY( -cameraForwardXY.y, cameraForwardXY.x , 0.0f );
	cbengine::Vector3<float> cameraMoveVector( 0.0f, 0.0f, 0.0f );

	EulerAngles	cameraRotation( 0.0f, 0.0f, 0.0f );

	// Handle Mouse Input
	POINT mousePoint;

	if ( GetCursorPos( &mousePoint ) ) {

		float xDistanceFromCenter = ( mousePoint.x - feedback::HALF_SCREEN_WIDTH );
		float yDistanceFromCenter = ( mousePoint.y - feedback::HALF_SCREEN_HEIGHT ); 

		float xDistanceDampened = xDistanceFromCenter * MOUSE_DAMPENING_FACTOR;
		float yDistanceDampened = yDistanceFromCenter * MOUSE_DAMPENING_FACTOR;

		cbengine::clampFloat( -MAXIMUM_MOUSE_POSITION_DISPLACEMENT_AFTER_DAMPENING, 
			MAXIMUM_MOUSE_POSITION_DISPLACEMENT_AFTER_DAMPENING, 
			xDistanceFromCenter );


		cbengine::clampFloat( -MAXIMUM_MOUSE_POSITION_DISPLACEMENT_AFTER_DAMPENING, 
			MAXIMUM_MOUSE_POSITION_DISPLACEMENT_AFTER_DAMPENING, 
			yDistanceFromCenter );

		cameraRotation.yawDegreesAboutZ		-= xDistanceDampened;
		cameraRotation.pitchDegreesAboutY	+= yDistanceDampened;

	} // end if


	// Handle Keyboard Input
	InputHandler& sharedInputHandler = InputHandler::getSharedInputHandler();
	const bool * virtualKeys = sharedInputHandler.getKeyboardStatus();

	if ( virtualKeys[ 'W' ] ) {

		cameraMoveVector += cameraForwardXY;

	}

	if ( virtualKeys [ 'D' ] ) {

		cameraMoveVector -= cameraLeftXY;

	}

	if ( virtualKeys[ 'S' ] ) {

		cameraMoveVector -= cameraForwardXY;

	}

	if ( virtualKeys[ 'A' ] ) {

		cameraMoveVector += cameraLeftXY;	

	}

	if ( virtualKeys[ 'R' ] ) {

		cameraMoveVector.z += 1.0f;

	}

	if ( virtualKeys[ 'F' ] ) {

		cameraMoveVector.z -= 1.0f;

	}

	if ( virtualKeys[ VK_LEFT ] ) {

		cameraRotation.yawDegreesAboutZ += 1.0f;

	}

	if ( virtualKeys[ VK_RIGHT ] ) {

		cameraRotation.yawDegreesAboutZ -= 1.0f;

	}

	cameraMoveVector *= ( CAMERA_VELOCITY_PER_SECOND * deltaSeconds );

	cameraRotation.yawDegreesAboutZ		*= ( CAMERA_ROTATION_DEGREES_PER_SECOND * deltaSeconds );
	cameraRotation.pitchDegreesAboutY	*= ( CAMERA_ROTATION_DEGREES_PER_SECOND * deltaSeconds );

	m_position += cameraMoveVector;

	m_orientation.yawDegreesAboutZ		+= cameraRotation.yawDegreesAboutZ;
	m_orientation.pitchDegreesAboutY	+= cameraRotation.pitchDegreesAboutY;

	cbengine::clampFloat( feedback::PITCH_ORIENTATION_MIN, 
		feedback::PITCH_ORIENTATION_MAX,
		m_orientation.pitchDegreesAboutY );

	SetCursorPos( static_cast<int>( feedback::HALF_SCREEN_WIDTH ), 
		static_cast<int>( feedback::HALF_SCREEN_HEIGHT ) );

} // endProcessMouseandKeyboard


void Camera::setCameraDefaults() {

	m_orientation.yawDegreesAboutZ = 0.0f;
	m_orientation.pitchDegreesAboutY = 0.0f;
	m_orientation.rollDegreesAboutX = 0.0f;

	m_position.x = 0.0f;
	m_position.y = 0.0f;
	m_position.z = 0.0f;

} // end setDefaults