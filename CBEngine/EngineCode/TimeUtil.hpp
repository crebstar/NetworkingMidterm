
#ifndef included_TimeUtil
#define included_TimeUtil
#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace cbutil {

// Constants and Global Variables
const double FRAME_TIME_SECONDS = 1.0 / 60.0;
extern double g_secondsPerCount;

inline void initializeTimeSystem() {
	// Get the amount of clock cycles per second 
	if ( g_secondsPerCount == 0.0 ) {

		LARGE_INTEGER countsPerSecond;
		QueryPerformanceFrequency( &countsPerSecond );
		g_secondsPerCount = 1.0 / static_cast<double>( countsPerSecond.QuadPart );
	}  // end if
}


inline double getCurrentTimeSeconds() {
	// Get the total clock cycles since the CPU has been awake
	LARGE_INTEGER performanceCount;
	QueryPerformanceCounter( &performanceCount );

	return static_cast<double>( performanceCount.QuadPart ) * g_secondsPerCount;
} 

} // end namespace


#endif