#include "Clock.hpp"

#include <cassert>

#include "Console.hpp"
#include "TimeSystem.hpp"
#include "Alarm.hpp"
#include "MathUtil.hpp"
#include "Vector4.hpp"
#include "CBStringHelper.hpp"


#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

STATIC int Clock::currentClockCount = 0;

Clock::~Clock() {

	--currentClockCount;

	cleanUpAndFreeMemoryForAlarms();
}


void Clock::cleanUpAndFreeMemoryForAlarms() {

	std::map<std::string,Alarm*>::iterator itAlarm;
	for ( itAlarm = m_alarmRegistry.begin(); itAlarm != m_alarmRegistry.end(); ++itAlarm ) {

		Alarm* alarmToDelete = itAlarm->second;
		delete alarmToDelete;
	}
}

Clock::Clock( Clock* parentClock, std::string clockName ) {

	setClockDefaults();

	if ( currentClockCount > 0 ) {
		// Only root clock may have a nullptr parent
		assert( parentClock != nullptr );
	}

	setParentClock( parentClock );
	m_clockName = clockName;

	TimeSystem& sharedTimeSystem = TimeSystem::getSharedTimeSystem();
	sharedTimeSystem.addClockToRegistry( clockName, this );

	++currentClockCount;
}


void Clock::advanceTime( double deltaSeconds ) {

	if ( !m_isPaused ) {

		m_currentFrameDeltaSeconds = ( deltaSeconds * m_timeScaleCoefficient );

		cbengine::clampDouble( 0.0, m_maxDeltaSeconds, m_currentFrameDeltaSeconds );

		m_timeElapsedSeconds += m_currentFrameDeltaSeconds;

		updateAlarms( m_currentFrameDeltaSeconds );

		for ( size_t i = 0; i < m_childrenClocks.size(); ++i ) {

			Clock* childClock = m_childrenClocks[i];
			childClock->advanceTime( m_currentFrameDeltaSeconds );
		}
	}
}


void Clock::increaseTimeScaleCoefficient( double timeScaleCoefficientIncrease ) {

	m_timeScaleCoefficient += timeScaleCoefficientIncrease;

	if ( m_timeScaleCoefficient < 0.0 ) {
		m_timeScaleCoefficient = 0.0;
	}
}


void Clock::addChildToClock( Clock* childToAdd ) {

	assert( childToAdd != nullptr ); // TODO:: replace with custom asserts
	m_childrenClocks.push_back( childToAdd );
}


bool Clock::removeChildClock( Clock* clockToRemove ) {

	assert( clockToRemove != nullptr );
	int indexOfClockToRemove = -1;

	for ( int i = 0; i < static_cast<int>( m_childrenClocks.size() ); ++i ) {

		Clock* childClock = m_childrenClocks[i];
		if ( childClock == clockToRemove ) {

			indexOfClockToRemove = i;
			break;
		}
	}

	if ( indexOfClockToRemove >= 0 ) {

		m_childrenClocks.erase( m_childrenClocks.begin() + indexOfClockToRemove );
		return true;
	} 

	return false;
}


// Alarms
bool Clock::createAlarm( const std::string& alarmName,
	float totalDurationSeconds,
	bool isRecurring,
	const std::string& eventNameToFire,
	const NamedProperties& namedPropertiesToInclude ) 
{
	// Can't have alarms with duplicate names
	std::map<std::string,Alarm*>::iterator itAlarm;
	itAlarm = m_alarmRegistry.find( alarmName );

	if ( itAlarm != m_alarmRegistry.end() ) {

		return false;
	}

	Alarm* alarmToCreate = new Alarm( alarmName, totalDurationSeconds, isRecurring, eventNameToFire, namedPropertiesToInclude );
	m_alarmRegistry.insert( std::pair<std::string,Alarm*>( alarmName, alarmToCreate ) );

	return true;
}


const Alarm* Clock::getAlarmWithName( const std::string& alarmName ) {

	Alarm* alarmToReturn = nullptr;
	std::map<std::string,Alarm*>::iterator itAlarm;
	
	itAlarm = m_alarmRegistry.find( alarmName );
	if ( itAlarm != m_alarmRegistry.end() ) {

		alarmToReturn = itAlarm->second;
	}

	return alarmToReturn;
}


void Clock::updateAlarms( double deltaSeconds ) {

	std::vector<std::string> alarmsToRemove;

	std::map<std::string,Alarm*>::iterator itAlarm;
	for ( itAlarm = m_alarmRegistry.begin(); itAlarm != m_alarmRegistry.end(); ++itAlarm ) {

		Alarm* alarmToUpdate = itAlarm->second;
		alarmToUpdate->advanceTime( static_cast<float>( deltaSeconds ) );

		if ( alarmToUpdate->isFlaggedForDeletion() ) {
			alarmsToRemove.push_back( alarmToUpdate->getAlarmName() );
		}
	}


	for ( size_t i = 0; i < alarmsToRemove.size(); ++i ) {

		const std::string alarmNameToRemove = alarmsToRemove[i];
		std::map<std::string,Alarm*>::iterator itDel;
		itDel = m_alarmRegistry.find( alarmNameToRemove );

		if ( itDel != m_alarmRegistry.end() ) {

			Alarm* alarmToDelete = itDel->second;
			delete alarmToDelete;

			m_alarmRegistry.erase( itDel );
		}
	}
}


void Clock::setClockDefaults() {

	m_clockIncludedInHierarchy = false;
	m_currentFrameDeltaSeconds = 0.0f;
	m_timeElapsedSeconds = 0.0f;
	setClockPaused( false );
	setTimeScaleCoefficient( CLOCK_DEFAULT_TIME_SCALE_COEFFICIENT );
	setMaxDeltaSeconds( CLOCK_DEFAULT_MAX_TIME_DELTASECONDS );
	setParentClock( nullptr );
}

// DEBUG
void Clock::outputSelfAndChildrenDepthFirstToConsole() {

	Console* sharedConsole = Console::getSharedDeveloperConsole();
	cbengine::Vector4<float> textColor( 0.90f, 0.30f, 0.67f, 1.0f );
	std::string consoleLine = m_clockName;

	std::string clockElapsedTime = std::to_string( static_cast<long double>( m_timeElapsedSeconds ) );
	consoleLine += " - ";
	consoleLine += clockElapsedTime;

	sharedConsole->addStringToConsole( consoleLine, textColor );

	for ( size_t i = 0; i < m_childrenClocks.size(); ++i ) {

		Clock* childClock = m_childrenClocks[i];
		childClock->outputSelfAndChildrenDepthFirstToConsole();
	}
}