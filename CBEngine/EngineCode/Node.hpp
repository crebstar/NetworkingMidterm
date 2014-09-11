


#ifndef included_Node
#define included_Node
#pragma once

/*
	The lowest level representation of a game object. A node is an entity in a game that has
	a position, orientation, and other basic data that describe a basic game object. It does not
	have a defined texture.
*/

#include <set>
#include <cassert>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/gL.h>

#include "EngineMacros.hpp"
#include "Vector2.hpp"
#include "Size.hpp"
#include "AABB.hpp"

namespace cbengine {

// Implemented as conceptual const
class Node {

public:

	virtual ~Node();

	Node();

	// ABSTRACT Core Methods
	virtual void update( float deltaSeconds ) = 0;
	virtual void render() const				  = 0;

	
	// Motion and Rotation
	virtual void applyForwardVelocityDelta( float nodeAcceleration, float velocityScaleFactor, float deltaSeconds );
	virtual void setForwardVelocity( float maxNodeVelocity, float velocityScaleFactor );
	virtual void applyVelocityForTimeStep( float deltaSeconds );
	virtual void interpolateCurrentTowardsDesiredOrientation( float maxRotationalVelocityPerSecond, float deltaSeconds );

	virtual void addChild( Node * childToAdd);

	virtual void checkAndClampNodePosition( const Size & configurationSpace );

	// Mutators
	virtual void setDesiredOrientationDegrees( float desiredDegrees );

	virtual inline float getDesiredOrientationDegrees() const {
		return m_desiredOrientationDegrees;
	}

	virtual inline void setContentSize( const Size & contentSize ) {
		m_ContentSize.m_width = contentSize.m_width;
		m_ContentSize.m_height = contentSize.m_height;
	}

	virtual inline void setContentSize( float width, float height ) {
		m_ContentSize.m_width = width;
		m_ContentSize.m_height = height;
	}

	virtual inline const Size & getContentSize() const {
		return m_ContentSize;
	}

	virtual inline const AABB & getBoundingBox() const {
		return m_AABB;
	}

	virtual inline void setPosition( const Vector2 & position ) {
		m_position.x = position.x;
		m_position.y = position.y;
		m_AABB.m_origin.x = position.x;
		m_AABB.m_origin.y = position.y;
	}

	virtual inline void setPosition( float xPos, float yPos ) {
		m_position.x = xPos;
		m_position.y = yPos;
		m_AABB.m_origin.x = xPos;
		m_AABB.m_origin.y = yPos;
	}

	virtual inline const Vector2 & getPosition() const {
		return m_position;
	}

	virtual inline void setRadius( float radius ) {
		m_radius = radius;
	}

	virtual inline float getRadius() const {
		return m_radius;
	}

	virtual inline void setOrientationDegrees( float orientationDegrees ) {
		m_orientationDegrees = orientationDegrees;
	}

	virtual inline float getOrientationDegrees() const {
		return m_orientationDegrees;
	}

	virtual inline void setVelocity( const Vector2 & velocity ) {
		m_velocity.x = velocity.x;
		m_velocity.y = velocity.y;
	}

	virtual inline void setVelocity( float xVel, float yVel ) {
		m_velocity.x = xVel;
		m_velocity.y = yVel;
	}

	virtual inline const Vector2 & getVelocity() const {
		return m_velocity;
	}

	virtual inline void setZOrder( int zOrder ) {
		m_zOrder = zOrder;
	}

	virtual inline int getZOrder() const {
		return m_zOrder;
	}

	virtual inline void setNodeParent( Node * parentNode ) {
		if (parentNode != nullptr )
			m_parent = parentNode;
	}

	virtual inline void setDirty( bool dirty ) {
		m_dirty = dirty;
	}

	virtual inline bool isDirty() const {
		return m_dirty;
	}

	PREVENT_COPY_AND_ASSIGN(Node);

protected:

	virtual void drawChildren() const;
		
	// Instance Variables
	Node *				m_parent;
	std::set<Node *>	m_childrenNodes;
	Size				m_ContentSize;
	Vector2				m_position;
	float				m_radius;
	AABB				m_AABB;
	float				m_orientationDegrees;
	float				m_desiredOrientationDegrees;
	Vector2				m_velocity;				// World Units Per Second
	
	int					m_zOrder;

	mutable bool		m_dirty;

private:

	
};


} // end namespace

#endif