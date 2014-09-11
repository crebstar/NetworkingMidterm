#ifndef included_Clock
#define included_Clock
#pragma once

#include <vector>
#include <map>
#include <string>

#include "EngineMacros.hpp"
#include "NamedProperties.hpp"

class Alarm;

class Clock {
public:
	friend class TimeSystem;

	static int currentClockCount;

	~Clock();
	Clock( Clock* parentClock, std::string clockName );

	// Core
	void advanceTime( double deltaSeconds );

	// Convenience Functions
	void addChildToClock( Clock* childToAdd );
	void increaseTimeScaleCoefficient( double timeScaleCoefficientIncrease );

	// Alarms
	bool createAlarm( const std::string& alarmName,
		float totalDurationSeconds,
		bool isRecurring,
		const std::string& eventNameToFire,
		const NamedProperties& namedPropertiesToInclude );
	
	const Alarm* getAlarmWithName( const std::string& alarmName );

	void updateAlarms( double deltaSeconds );
	void cleanUpAndFreeMemoryForAlarms();

	// Inline Mutators
	void setParentClock( Clock* parentClock );
	const Clock* getParentClock() const;
	const std::vector<Clock*>& getChildrenOfClock() const;
	const std::string& getClockName() const;

	double getTimeElapsedSeconds() const;
	double getCurrentFrameDeltaSeconds() const;
	void setTimeScaleCoefficient( double timeScaleCoefficient );
	double getTimeScaleCoefficient() const;
	void setMaxDeltaSeconds( double maxDeltaSeconds );
	double getMaxDeltaSeconds() const;
	void setClockPaused( bool isClockPaused );
	bool isClockPaused() const;
	bool isClockInClockHierarchy() const;

protected:
	bool removeChildClock( Clock* clockToRemove );

	void setClockDefaults();

	// Debug
	void outputSelfAndChildrenDepthFirstToConsole();
private:
	PREVENT_COPY_AND_ASSIGN( Clock );

	Clock*							m_parentClock;
	std::vector<Clock*>				m_childrenClocks;
	std::string						m_clockName;
	
	std::map<std::string,Alarm*>	m_alarmRegistry;

	double							m_timeElapsedSeconds;
	double							m_currentFrameDeltaSeconds;
	double							m_timeScaleCoefficient;
	double							m_maxDeltaSeconds;

	bool							m_isPaused;
	mutable bool					m_clockIncludedInHierarchy;
};

// Inline Mutators
inline void Clock::setParentClock( Clock* parentClock ) {

	m_parentClock = parentClock;
}


inline const Clock* Clock::getParentClock() const {

	return m_parentClock;
}


inline const std::vector<Clock*>& Clock::getChildrenOfClock() const {

	return m_childrenClocks;
}


inline const std::string& Clock::getClockName() const {

	return m_clockName;
}

inline double Clock::getTimeElapsedSeconds() const {

	return m_timeElapsedSeconds;
}


inline double Clock::getCurrentFrameDeltaSeconds() const {

	return m_currentFrameDeltaSeconds;
}


inline void Clock::setTimeScaleCoefficient( double timeScaleCoefficient ) {

	m_timeScaleCoefficient = timeScaleCoefficient;
}


inline double Clock::getTimeScaleCoefficient() const {

	return m_timeScaleCoefficient;
}


inline void Clock::setMaxDeltaSeconds( double maxDeltaSeconds ) {

	m_maxDeltaSeconds = maxDeltaSeconds;
}


inline double Clock::getMaxDeltaSeconds() const {

	return m_maxDeltaSeconds;
}


inline void Clock::setClockPaused( bool isClockPaused ) {

	m_isPaused = isClockPaused;
}


inline bool Clock::isClockPaused() const {

	return m_isPaused;
}


inline bool Clock::isClockInClockHierarchy() const {
	
	return m_clockIncludedInHierarchy;
}

#endif