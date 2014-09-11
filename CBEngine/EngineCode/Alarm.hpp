#ifndef included_Alarm
#define included_Alarm
#pragma once

#include <string>

#include "EngineMacros.hpp"
#include "NamedProperties.hpp"

class Alarm {
public:
	~Alarm();
	explicit Alarm( const std::string& alarmName,
		float durationForAlarm, 
		bool isRecurring, 
		const std::string& eventNameToFire, 
		const NamedProperties& propertiesToInclude );

	// Core Functions
	void advanceTime( float durationToAdvance );
	void initializeAndStartAlarm();
	void triggerAlarm();

	// Inline Mutators
	void setIsRecurring( bool isRecurring );
	bool isRecurringAlarm() const;
	bool isAlarmRunning() const;
	bool isFlaggedForDeletion() const;
	void setNamedProperties( const NamedProperties& namedProperties );
	const NamedProperties& getNamedProperties() const;
	void setEventNameToFire( const std::string& eventNameToFire );
	const std::string& getEventNameToFire() const;
	void setAlarmName( const std::string& alarmName );
	const std::string& getAlarmName() const;
	void setDurationForAlarm( float durationSeconds );
	float getTotalDurationForAlarm() const;
	float getPercentElapsedTime() const;
	float getPercentRemainingTime() const;
	float getSecondsRemaining() const;
	float getSecondsElapsed() const;

protected:

	void setAlarmDefaults();
private:
	PREVENT_COPY_AND_ASSIGN( Alarm );

	float					m_totalDurationForAlarmSeconds;
	double					m_startTimeSeconds;
	float					m_currentDurationElapsedSeconds;
	double					m_endTimeSeconds;
	bool					m_isRecurring;
	mutable bool		    m_isRunning;
	mutable bool			m_flaggedForDeletion;
	NamedProperties			m_namedPropertiesForEvent;
	std::string				m_eventNameToFire;
	std::string				m_alarmName;
};

// Inline Functions
inline void Alarm::setIsRecurring( bool isRecurring ) {

	m_isRecurring = isRecurring;
}


inline bool Alarm::isRecurringAlarm() const {

	return m_isRecurring;
}


inline bool Alarm::isAlarmRunning() const {

	return m_isRunning;
}


inline bool Alarm::isFlaggedForDeletion() const {

	return m_flaggedForDeletion;
}


inline void Alarm::setNamedProperties( const NamedProperties& namedProperties ) {

	m_namedPropertiesForEvent = namedProperties;
}


inline const NamedProperties& Alarm::getNamedProperties() const {

	return m_namedPropertiesForEvent;
}


inline void Alarm::setEventNameToFire( const std::string& eventNameToFire ) {

	m_eventNameToFire = eventNameToFire;
}


inline const std::string& Alarm::getEventNameToFire() const {

	return m_eventNameToFire;
}


inline void Alarm::setAlarmName( const std::string& alarmName ) {

	m_alarmName = alarmName;
}


inline const std::string& Alarm::getAlarmName() const {

	return m_alarmName;
}


inline void Alarm::setDurationForAlarm( float durationSeconds ) {

	if ( durationSeconds < 0.0f ) {
		durationSeconds = 0.0f;
	}

	m_totalDurationForAlarmSeconds = durationSeconds;
}


inline float Alarm::getTotalDurationForAlarm() const {

	return m_totalDurationForAlarmSeconds;
}


inline float Alarm::getPercentElapsedTime() const {

	if ( ( m_startTimeSeconds - m_endTimeSeconds ) <= 0.0 ) {

		return 0.0f;
	}

	float percentDurationElapsed = ( m_currentDurationElapsedSeconds / static_cast<float>( m_startTimeSeconds - m_endTimeSeconds ) );

	return percentDurationElapsed;
}


inline float Alarm::getPercentRemainingTime() const {

	if ( ( m_startTimeSeconds - m_endTimeSeconds ) <= 0.0 ) {

		return 0.0f;
	}

	float timeRemainingSeconds = getSecondsRemaining();
	float percentTimeRemaining = ( timeRemainingSeconds / static_cast<float>( m_startTimeSeconds - m_endTimeSeconds ) );

	return percentTimeRemaining;
}


inline float Alarm::getSecondsRemaining() const {

	float timeRemainingSeconds = m_totalDurationForAlarmSeconds - m_currentDurationElapsedSeconds;

	return timeRemainingSeconds;
}


inline float Alarm::getSecondsElapsed() const {

	return m_currentDurationElapsedSeconds;
}

#endif
