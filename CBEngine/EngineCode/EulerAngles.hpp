
#ifndef included_EulerAngles
#define included_EulerAngles
#pragma once

#include "EngineMacros.hpp"

class EulerAngles {

public:

	~EulerAngles() {


	}

	EulerAngles( float roll, float pitch, float yaw ) {

		yawDegreesAboutZ = yaw;
		pitchDegreesAboutY = pitch;
		rollDegreesAboutX = roll;

	}

	EulerAngles() {

		yawDegreesAboutZ = 0.0f;
		pitchDegreesAboutY = 0.0f;
		rollDegreesAboutX = 0.0f;

	}

	float		yawDegreesAboutZ;
	float		pitchDegreesAboutY;
	float		rollDegreesAboutX;

protected:


private:

	PREVENT_COPY_AND_ASSIGN( EulerAngles );
};

#endif