
#ifndef included_Entity
#define included_Entity
#pragma once

#include "Particle.hpp"

#include <vector>
#include <string>
#include <cassert>

#include "EngineCommon.hpp"
#include "Vertex.hpp"

#include "Texture.hpp"
#include "TextureManager.hpp"
#include "OpenGLShaderProgram.hpp"

#include "Material.hpp"

class Entity3D : public Particle {
public:

	static Entity3D* createEntityWithCubeGeometry( float xlength, float ywidth, float zheight, const std::string & diffuseTextureName );
	static Entity3D* createEntityWithSphereGeometry( float radius, float slices, float stacks, const std::string & diffuseTextureName );
	static Entity3D* createEntityWithFullScreenQuad( float screenWidth, float screenHeight );
	static Entity3D* createEntityFromVertexVector( const std::vector< cbengine::Vertex >& vertVec );
	static Entity3D* createEntityFromVertexVector( const std::vector<cbengine::Vertex>& vertVec, const std::string& diffuseTextureName );

	virtual ~Entity3D();
	Entity3D();
	
	virtual void update( float deltaSeconds );
	virtual void render( float deltaSeconds ) const;

	void createMaterial();

	std::vector< cbengine::Vertex >				m_verts;
	cbengine::Texture *							m_diffuseTexture;
	unsigned int								m_VBOBufferID;
	Material *									m_material;

protected:

private:
	PREVENT_COPY_AND_ASSIGN( Entity3D );

	static void computeTangentAndBitangentVectorsForVerts( Entity3D * entity );

};




#endif