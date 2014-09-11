#ifndef included_MatrixStack
#define included_MatrixStack
#pragma once

#include <stack>
#include <math.h>

#include "..\EngineCode\EngineCommon.hpp"

#include "..\EngineCode\Matrix44.hpp"
#include "..\EngineCode\Vector3D.hpp"
#include "MathUtil.hpp"

class MatrixStack {
public:

	static MatrixStack* sharedMatrixStack() {
		static MatrixStack matrixStack;
		return &matrixStack;
	}

	~MatrixStack();

	// Inline Functions
	void pushMatrixToStack( const Matrix44<float> & matrixToPush );
	void popFromTopOfStack();
	void emptyCurrentMatrixStack();
	void emptyCurrentMatrixStackAndPushIdentityMatrix();
	const Matrix44<float> & getMatrixFromTopOfStack();
	void multiplyMatrixToTopOfStackAndPush( const Matrix44<float> & matrixToMult );

	// Rotation
	void applyRotationAboutXAndPushToStack( float angleRadians );
	void applyRotationAboutYAndPushToStack( float angleRadians );
	void applyRotationAboutZAndPushToStack( float angleRadians );

	// Transform
	void applyTranslationAndPushToStack( const cbengine::Vector3<float> & translationCoords );
	void applyTransformAndPushToStack( const Matrix44<float>& transformMatrix );

	// Perspective
	void createPerspectiveMatrixAndPushToStack( float fovy, float aspect, float nearDist, float farDist );
	void createOrthoMatrixAndPushToStack( float left, float right, float bottom, float top, float Znear, float Zfar );

	// Camera
	void setWorldToCameraSpaceTransformMatrix( const Matrix44<float>& worldToCameraSpaceTransformMatrix );
	const Matrix44<float>& getWorldToCameraSpaceTransformMatrix() const;
	void setCameraToWorldSpaceTransformMatrix( const Matrix44<float>& cameraToWorldSpaceTransformMatrix );
	const Matrix44<float>& getCameraToWorldSpaceTransformMatrix() const;
	void setCurrentCameraPosition( const cbengine::Vector3<float>& currentCameraPosition );
	const cbengine::Vector3<float> getCurrentCameraPosition() const;

	// Model
	void setCurrentModelTransformMatrix( const Matrix44<float>& modelTransformMatrix );
	const Matrix44<float>& getCurrentModelTransformMatrix() const;

	// Projection
	void setCurrentProjectionMatrix( const Matrix44<float>& projectionMatrix );
	const Matrix44<float>& getCurrentProjectionMatrix() const;

protected:

private:
	PREVENT_COPY_AND_ASSIGN( MatrixStack );

	MatrixStack() {

		m_cameraToWorldSpaceTransformMatrix = Matrix44<float>::createIdentityMatrixFloat();
		m_worldToCameraSpaceTransformMatrix = Matrix44<float>::createIdentityMatrixFloat();
		m_currentModelTransformMatrix = Matrix44<float>::createIdentityMatrixFloat();
		m_currentProjectionMatrix = Matrix44<float>::createIdentityMatrixFloat();
	}

	std::stack< Matrix44<float> >		m_matrixStack;
	Matrix44<float>						m_cameraToWorldSpaceTransformMatrix;
	Matrix44<float>						m_worldToCameraSpaceTransformMatrix;
	Matrix44<float>						m_currentModelTransformMatrix;
	Matrix44<float>						m_currentProjectionMatrix;
	cbengine::Vector3<float>			m_currentCameraPosition;
};

// Inline Functions
inline void MatrixStack::pushMatrixToStack( const Matrix44<float> & matrixToPush ) {
	m_matrixStack.push( matrixToPush );
}


inline void MatrixStack::popFromTopOfStack() {
	m_matrixStack.pop();
}


inline const Matrix44<float> & MatrixStack::getMatrixFromTopOfStack() {

	const Matrix44<float> & topMatrix = m_matrixStack.top();
	return topMatrix;
}


inline void MatrixStack::emptyCurrentMatrixStack() {

	while ( !m_matrixStack.empty() ) {
		m_matrixStack.pop();
	}
}


inline void MatrixStack::emptyCurrentMatrixStackAndPushIdentityMatrix() {

	while ( !m_matrixStack.empty() ) {
		m_matrixStack.pop();
	}
	Matrix44<float> identityMatrix = Matrix44<float>::createIdentityMatrixFloat();
	m_matrixStack.push( identityMatrix );

}

inline void MatrixStack::multiplyMatrixToTopOfStackAndPush( const Matrix44<float> & matrixToMult ) {

	const Matrix44<float> & topOfStackMatrix = m_matrixStack.top();
	Matrix44<float> resultingMatrix;
	Matrix44<float>::matrixMultiply( matrixToMult, topOfStackMatrix, resultingMatrix );
	m_matrixStack.push( resultingMatrix );
}

// Rotation
inline void MatrixStack::applyRotationAboutXAndPushToStack( float angleRadians ) {

	Matrix44<float> rotationAboutX = Matrix44<float>::createCannonicalRotationAboutXMatrixFloat( angleRadians );
	const Matrix44<float> & modelViewMatrix = m_matrixStack.top();
	Matrix44<float> resultingTransform;
	Matrix44<float>::matrixMultiply( rotationAboutX, modelViewMatrix, resultingTransform );
	m_matrixStack.push( resultingTransform );
}


inline void MatrixStack::applyRotationAboutYAndPushToStack( float angleRadians ) {

	Matrix44<float> rotationAboutY = Matrix44<float>::createCannonicalRotationAboutYMatrixFloat( angleRadians );
	const Matrix44<float> & modelViewMatrix = m_matrixStack.top();
	Matrix44<float> resultingTransform;
	Matrix44<float>::matrixMultiply( rotationAboutY, modelViewMatrix, resultingTransform );
	m_matrixStack.push( resultingTransform );
}


inline void MatrixStack::applyRotationAboutZAndPushToStack( float angleRadians ) {

	Matrix44<float> rotationAboutZ = Matrix44<float>::createCannonicalRotationAboutZMatrixFloat( angleRadians );
	const Matrix44<float> & modelViewMatrix = m_matrixStack.top();
	Matrix44<float> resultingTransform;
	Matrix44<float>::matrixMultiply( rotationAboutZ, modelViewMatrix, resultingTransform );
	m_matrixStack.push( resultingTransform );
}

// Transform
inline void MatrixStack::applyTranslationAndPushToStack( const cbengine::Vector3<float> & translationCoords ) {

	Matrix44<float> translationMatrix = Matrix44<float>::createTranslateMatrixFloat( translationCoords );
	const Matrix44<FLOAT> & modelViewMatrix = m_matrixStack.top();
	Matrix44<float> resultingTransform;
	Matrix44<float>::matrixMultiply( translationMatrix, modelViewMatrix, resultingTransform );
	m_matrixStack.push( resultingTransform );
}


inline void MatrixStack::applyTransformAndPushToStack( const Matrix44<float>& transformMatrix ) {

	const Matrix44<FLOAT> & modelViewMatrix = m_matrixStack.top();
	Matrix44<float> resultingTransform;
	Matrix44<float>::matrixMultiply( transformMatrix, modelViewMatrix , resultingTransform );
	m_matrixStack.push( resultingTransform );
}

// Perspective
inline void MatrixStack::createPerspectiveMatrixAndPushToStack( float fovy, float aspect, float nearDist, float farDist ) {

	Matrix44<float> perspectiveMatrix;

	float ymax = nearDist * tan(fovy * cbengine::PI / 360.0f );
	float ymin = -ymax;
	float xmax = ymax * aspect;
	float xmin = ymin * aspect;

	float width = xmax - xmin;
	float height = ymax - ymin;

	float depth = farDist - nearDist;
	float q = -(farDist + nearDist) / depth;
	float qn = -2 * (farDist * nearDist) / depth;

	float w = 2 * nearDist / width;
	float h = 2 * nearDist / height;

	// Col 1
	perspectiveMatrix.matrixData[0] = w;
	perspectiveMatrix.matrixData[1] = 0.0f;
	perspectiveMatrix.matrixData[2] = 0.0f;
	perspectiveMatrix.matrixData[3] = 0.0f;
	// Col 2
	perspectiveMatrix.matrixData[4] = 0.0f;
	perspectiveMatrix.matrixData[5] = h;
	perspectiveMatrix.matrixData[6] = 0.0f;
	perspectiveMatrix.matrixData[7] = 0.0f;
	// Col3
	perspectiveMatrix.matrixData[8] = 0.0f;
	perspectiveMatrix.matrixData[9] = 0.0f;
	perspectiveMatrix.matrixData[10] = q;
	perspectiveMatrix.matrixData[11] = -1.0f;
	// Col4
	perspectiveMatrix.matrixData[12] = 0.0f;
	perspectiveMatrix.matrixData[13] = 0.0f;
	perspectiveMatrix.matrixData[14] = qn;
	perspectiveMatrix.matrixData[15] = 1.0f;

	const Matrix44<float>& topMatrix = m_matrixStack.top();
	Matrix44<float> resultingMatrix;
	Matrix44<float>::matrixMultiply( perspectiveMatrix, topMatrix, resultingMatrix );
	m_matrixStack.push( resultingMatrix );

	m_currentProjectionMatrix = resultingMatrix;
}


inline void MatrixStack::createOrthoMatrixAndPushToStack( float left, float right, float bottom, float top, float Znear, float Zfar ) {

	Matrix44<float> orthoMatrix;
	// Col 1
	orthoMatrix.matrixData[0] = 2.0f / ( right - left );
	orthoMatrix.matrixData[1] = 0.0f;
	orthoMatrix.matrixData[2] = 0.0f;
	orthoMatrix.matrixData[3] = 0.0f;
	// Col 2
	orthoMatrix.matrixData[4] = 0.0f;
	orthoMatrix.matrixData[5] = 2.0f / ( top - bottom );
	orthoMatrix.matrixData[6] = 0.0f;
	orthoMatrix.matrixData[7] = 0.0f;
	// Col 3
	orthoMatrix.matrixData[8] = 0.0f;
	orthoMatrix.matrixData[9] = 0.0f;
	orthoMatrix.matrixData[10] = -2.0f / ( Zfar - Znear);
	orthoMatrix.matrixData[11] = 0.0f;
	// Col 4
	orthoMatrix.matrixData[12] = -( right + left ) / ( right - left );
	orthoMatrix.matrixData[13] = -( top + bottom ) / ( top - bottom );
	orthoMatrix.matrixData[14] = -( Zfar + Znear ) / ( Zfar - Znear );
	orthoMatrix.matrixData[15] = 1.0f;
	
	const Matrix44<float>& topMatrix = m_matrixStack.top();
	Matrix44<float> resultingMatrix;
	Matrix44<float>::matrixMultiply( orthoMatrix, topMatrix, resultingMatrix );
	m_matrixStack.push( resultingMatrix );

}

inline void MatrixStack::setWorldToCameraSpaceTransformMatrix( const Matrix44<float>& worldToCameraSpaceTransformMatrix ) {
	
	m_worldToCameraSpaceTransformMatrix = worldToCameraSpaceTransformMatrix;
}


inline const Matrix44<float>& MatrixStack::getWorldToCameraSpaceTransformMatrix() const {

	return m_worldToCameraSpaceTransformMatrix;
}


inline void MatrixStack::setCameraToWorldSpaceTransformMatrix( const Matrix44<float>& cameraToWorldSpaceTransformMatrix ) {

	m_cameraToWorldSpaceTransformMatrix = cameraToWorldSpaceTransformMatrix;
}


inline const Matrix44<float>& MatrixStack::getCameraToWorldSpaceTransformMatrix() const {

	return m_cameraToWorldSpaceTransformMatrix;
}


inline void MatrixStack::setCurrentCameraPosition( const cbengine::Vector3<float>& currentCameraPosition ) {

	m_currentCameraPosition = currentCameraPosition;
}


inline const cbengine::Vector3<float> MatrixStack::getCurrentCameraPosition() const {

	return m_currentCameraPosition;
}


inline void MatrixStack::setCurrentModelTransformMatrix( const Matrix44<float>& modelTransformMatrix ) {

	m_currentModelTransformMatrix = modelTransformMatrix;
}


inline const Matrix44<float>& MatrixStack::getCurrentModelTransformMatrix() const {

	return m_currentModelTransformMatrix;
}


inline void MatrixStack::setCurrentProjectionMatrix( const Matrix44<float>& projectionMatrix ) {

	m_currentProjectionMatrix = projectionMatrix;
}


inline const Matrix44<float>& MatrixStack::getCurrentProjectionMatrix() const {

	return m_currentProjectionMatrix;
}

#endif