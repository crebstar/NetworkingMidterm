
#ifndef included_GameConstants
#define included_GameConstants
#pragma once

#include <string>

// Keep close eye on this
namespace feedback {

	// Perspective parameters
	const double VERTICAL_FIELD_OF_VIEW				= 50.625;
	const double ASPECT_RATIO						= ( 16.0 / 9.0 );
	const double NEAR_CLIPPING_PLANE				= 0.10;
	const double FAR_CLIPPING_PLANE					= 1000.0;

	const float HALF_SCREEN_WIDTH					= 900.0f;
	const float HALF_SCREEN_HEIGHT					= 450.0f;
	const float PITCH_ORIENTATION_MAX				= 88.0f;
	const float PITCH_ORIENTATION_MIN				= -88.0f;

	const float SCREEN_WIDTH						= 500.0f;
	const float SCREEN_HEIGHT						= 500.0f;
	const float SCREEN_ADJUSTMENT					= 50.0f;

	const double ORTHO_SCREEN_WIDTH					= 500.0f;
	const double ORTHO_SCREEN_HEIGHT				= 500.0f;

	const float  CONSOLE_RESET_DUR					= 0.3f;

} // end namespace

#endif