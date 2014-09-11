
#ifndef included_SphereShape
#define included_SphereShape
#pragma once

#include "Particle.hpp"

#include <string>
#include <vector>

#include "EngineMacros.hpp"
#include "Vertex.hpp"
#include "Texture.hpp"


class SphereShape : public Particle {

public:

	virtual ~SphereShape();
	explicit SphereShape( float radius, float slices, float stacks, const std::string & diffuseTextureName );

	virtual void update( float deltaSeconds );
	virtual void render( float deltaSeconds ) const;

	std::vector< cbengine::Vertex >				m_verts;
	std::vector< unsigned short >				m_sphere_Indices;
	cbengine::Texture *							m_diffuseTexture;
	unsigned int								m_VBOBufferID;

	float m_radius;
	float m_slices;
	float m_stacks;

protected:


private:
	PREVENT_COPY_AND_ASSIGN( SphereShape );

	void generateSphereVBOData();

};

#endif