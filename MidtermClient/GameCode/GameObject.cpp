#include "GameObject.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/gl.h> // ugh get rid of this dependency 

#include "../../CBEngine/EngineCode/Vector3D.hpp"
#include "../../CBEngine/EngineCode/MatrixStack.hpp"
#include "../../CBEngine/EngineCode/Matrix44.hpp"

GameObject::~GameObject() {

}


GameObject::GameObject() {

	setGameObjectDefaults();

	m_position.x = 100.0f;
	m_position.y = 100.0f;
	m_orientationDegrees = 0.0f;
	m_isFlag = false;
	m_collisionDisk.radius = COLLISION_DISK_RADIUS;
}


void GameObject::update( float deltaSeconds ) {

	updatePhysics( deltaSeconds );
}


void GameObject::updatePhysics( float deltaSeconds ) {

	// This is no longer needed with an authoritative server
	//m_currentVelocity.x = m_currentVelocity.x * MAX_VELOCITY_PER_SECOND;
	//m_currentVelocity.y = m_currentVelocity.y * MAX_VELOCITY_PER_SECOND;

	//m_position.x = m_position.x + ( m_currentVelocity.x * deltaSeconds );
	//m_position.y = m_position.y + ( m_currentVelocity.y * deltaSeconds );

	//m_desiredPosition = m_position;

	m_currentVelocity.x = 0.0f;
	m_currentVelocity.y = 0.0f;

	m_collisionDisk.origin = m_position;
}


void GameObject::updateDesiredPosition( float deltaSeconds ) {

	m_currentVelocity.x = m_currentVelocity.x * MAX_VELOCITY_PER_SECOND;
	m_currentVelocity.y = m_currentVelocity.y * MAX_VELOCITY_PER_SECOND;

	m_desiredPosition.x = m_desiredPosition.x + ( m_currentVelocity.x * deltaSeconds );
	m_desiredPosition.y = m_desiredPosition.y + ( m_currentVelocity.y * deltaSeconds );
}


void GameObject::render( float deltaSeconds ) const {

	UNUSED( deltaSeconds );

	glUseProgram(0);
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	MatrixStack* matrixStack = MatrixStack::sharedMatrixStack();
	cbengine::Vector3<float> translationToPosition( m_position.x, m_position.y, 0.0f );
	matrixStack->applyTranslationAndPushToStack( translationToPosition );
	float orientationRadians = cbengine::degreesToRadians( m_orientationDegrees );
	matrixStack->applyRotationAboutZAndPushToStack( orientationRadians );
	const Matrix44<float> & topOfStack = matrixStack->getMatrixFromTopOfStack();
	glLoadMatrixf( topOfStack.matrixData );

	glDisable( GL_TEXTURE_2D );

	glColor3f( m_playerColor.x, m_playerColor.y, m_playerColor.z );

	const float halfWidth = m_objectSize.m_width * 1.00;
	

	glBegin( GL_TRIANGLES ); {

		glVertex2f( halfWidth, 0.0f );

		glVertex2f( -halfWidth, m_objectSize.m_height );

		glVertex2f( -halfWidth, -m_objectSize.m_height );

		//glVertex2f( -halfWidth, m_objectSize.m_height );

	} glEnd();


	matrixStack->popFromTopOfStack();
	matrixStack->popFromTopOfStack();
}


void GameObject::increaseOrientationDegrees( float amountToIncrease ) {

	m_orientationDegrees += amountToIncrease;
	if ( m_orientationDegrees >= 360.0f ) {
		
		m_orientationDegrees = m_orientationDegrees - 360.0f;

	} else if ( m_orientationDegrees < 0.0f ) {

		m_orientationDegrees = 360.0f - m_orientationDegrees;
	}
}


void GameObject::setGameObjectDefaults() {

	m_orientationDegrees = 0.0f;
	m_objectSize.m_width = 7.0f;
	m_objectSize.m_height = 7.0f;
	m_playerID = -1;
}


