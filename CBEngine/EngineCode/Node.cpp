
#include "Node.hpp"

#include <math.h>

#include "MathUtil.hpp"

#include "GameDirector.hpp"

#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

cbengine::Node::~Node() {

	std::set<Node*>::iterator it;

	for ( it = m_childrenNodes.begin(); it != m_childrenNodes.end(); it++) {

		Node * nodeToDelete = *(it);
		delete nodeToDelete;

	} // end for

} // end dtor

cbengine::Node::Node() :
	m_orientationDegrees( 0.0f ),
	m_desiredOrientationDegrees( 0.0f ),
	m_zOrder( 0 ),
	m_radius( 0.0f ),
	m_parent( nullptr ),
	m_dirty( false )
{


} // end ctor


void cbengine::Node::applyForwardVelocityDelta( float nodeAcceleration, float velocityScaleFactor, float deltaSeconds ) {

	float deltaVelocityMag = nodeAcceleration * deltaSeconds * velocityScaleFactor;
	float orientationRadians = cbengine::degreesToRadians( m_orientationDegrees ); 

	cbengine::Vector2 deltaVelocity;
	deltaVelocity.x = cos( orientationRadians );
	deltaVelocity.y = sin( orientationRadians );
	
	deltaVelocity.x *= deltaVelocityMag;
	deltaVelocity.y *= deltaVelocityMag;

	m_velocity.x += deltaVelocity.x;
	m_velocity.y += deltaVelocity.y;

} // end applyForwardVelocity

void cbengine::Node::setForwardVelocity( float maxNodeVelocity, float velocityScaleFactor ) {

	float nodeVelocityMag = maxNodeVelocity * velocityScaleFactor;
	float orientationRadians = cbengine::degreesToRadians( m_orientationDegrees );

	cbengine::Vector2 newVelocity;
	newVelocity.x = nodeVelocityMag * cos( orientationRadians );
	newVelocity.y = nodeVelocityMag * sin( orientationRadians );

	m_velocity.x = newVelocity.x;
	m_velocity.y = newVelocity.y;

}


void cbengine::Node::applyVelocityForTimeStep( float deltaSeconds ) {

	m_position.x += m_velocity.x * deltaSeconds;
	m_position.y += m_velocity.y * deltaSeconds;
	// Also update AABB
	m_AABB.m_origin.x = m_position.x;
	m_AABB.m_origin.y = m_position.y;

} // end applyVelocityForTimeStep


void cbengine::Node::interpolateCurrentTowardsDesiredOrientation( float maxRotationalVelocityPerSecond, float deltaSeconds ) {

	float angularDisplacement = m_desiredOrientationDegrees - m_orientationDegrees;

	while (angularDisplacement > 180.0f) {
		angularDisplacement -= 360.0f;
	}

	while (angularDisplacement < -180.0f) {
		angularDisplacement += 360.0f;
	}

	float adjustedMaxRotationalVelocityPerSecond = maxRotationalVelocityPerSecond * deltaSeconds;

	if ( fabs( angularDisplacement ) > adjustedMaxRotationalVelocityPerSecond ) {

		if ( angularDisplacement < adjustedMaxRotationalVelocityPerSecond ) {

			m_orientationDegrees -= adjustedMaxRotationalVelocityPerSecond;

		}  // end inner if

		if ( angularDisplacement > adjustedMaxRotationalVelocityPerSecond ) {

			m_orientationDegrees += adjustedMaxRotationalVelocityPerSecond;

		} // end inner if

	} // end if


}

void cbengine::Node::drawChildren() const {

	std::set< Node *>::iterator it;

	for (it = m_childrenNodes.begin(); it != m_childrenNodes.end(); it++) {

		Node * childNode = *(it);
		childNode->render();

	} // end for

} // end drawChildren

void cbengine::Node::addChild(  Node * childToAdd ) {

	if ( childToAdd && (childToAdd->m_parent == nullptr) ) {

		m_childrenNodes.insert( childToAdd );
		childToAdd->m_parent = this;

	} // end if 

} // end addChild

void cbengine::Node::setDesiredOrientationDegrees( float desiredDegrees ) {

	m_desiredOrientationDegrees = desiredDegrees;

	// Restrict angle range to [0 - 360)
	while (m_desiredOrientationDegrees >= 360.0f) {

		m_desiredOrientationDegrees -= 360.0f;

	} // end while

	while (m_desiredOrientationDegrees < 0 ) {

		m_desiredOrientationDegrees += 360.0f;

	} // end while

} // end setter

void cbengine::Node::checkAndClampNodePosition( const cbengine::Size & configurationSize ) {

	if ( m_position.x < m_radius ) {

		m_position.x = m_radius;

	} else if ( m_position.x > (configurationSize.m_width - m_radius)) {

		m_position.x = configurationSize.m_width - m_radius;
	}

	if ( m_position.y < m_radius ) {

		m_position.y = m_radius;

	} else if ( m_position.y > (configurationSize.m_height - m_radius) ) {
		m_position.y = configurationSize.m_height - m_radius;
	}

} // end checkAndClampNodePosition