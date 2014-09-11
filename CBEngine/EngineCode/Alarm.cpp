#include "Alarm.hpp"

#include "TimeSystem.hpp"
#include "EventSystem.hpp"


#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

Alarm::~Alarm() {

}


Alarm::Alarm( const std::string& alarmName,
	float durationForAlarm, 
	bool isRecurring,
	const std::string& eventNameToFire,
	const NamedProperties& propertiesToInclude )
{
	setAlarmDefaults();

	setAlarmName( alarmName );
	setDurationForAlarm( durationForAlarm );
	setIsRecurring( isRecurring );
	setEventNameToFire( eventNameToFire );
	setNamedProperties( propertiesToInclude );
}


void Alarm::initializeAndStartAlarm() {

	TimeSystem& sharedTimeSystem = TimeSystem::getSharedTimeSystem();

	m_startTimeSeconds = sharedTimeSystem.getCurrentTimeSeconds();
	m_endTimeSeconds = m_startTimeSeconds + static_cast<double>( m_totalDurationForAlarmSeconds );
	m_currentDurationElapsedSeconds = 0.0f;
	m_isRunning = true;
}


void Alarm::advanceTime( float durationToAdvance ) {

	m_currentDurationElapsedSeconds += durationToAdvance;

	if ( m_currentDurationElapsedSeconds > m_totalDurationForAlarmSeconds ) {

		triggerAlarm();
	}
}


void Alarm::triggerAlarm() {

	cbengine::FireEvent( m_eventNameToFire, m_namedPropertiesForEvent );

	if ( m_isRecurring ) {
		
		initializeAndStartAlarm();
	} else {

		m_isRunning = false;
		m_flaggedForDeletion = true;
	}
}


void Alarm::setAlarmDefaults() {

	m_startTimeSeconds = 0.0f;
	m_totalDurationForAlarmSeconds = 0.0f;
	m_isRecurring = false;
	m_isRunning = false;
	m_flaggedForDeletion = false;
	m_endTimeSeconds = 0.0f;
	m_currentDurationElapsedSeconds = 0.0f;
}