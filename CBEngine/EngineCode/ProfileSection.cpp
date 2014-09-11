#include "ProfileSection.hpp"

#include "TimeSystem.hpp"


#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

ProfileSection::~ProfileSection() {

	if ( m_isScopedProfileSection ) {

		TimeSystem& sharedTimeSystem = TimeSystem::getSharedTimeSystem();
		m_endTimeSeconds = sharedTimeSystem.getCurrentTimeSeconds();

		m_durationOfProfileForFrameSeconds = static_cast<float>( m_endTimeSeconds - m_startTimeSeconds );

		EngineProfiler& sharedEngineProfiler = EngineProfiler::getSharedEngineProfiler();
		sharedEngineProfiler.reportProfileSectionResult( *(this) ); 
	}
}


ProfileSection::ProfileSection( ProfileCategory profileCategory, bool isScopedProfileSection ) {

	setProfileSectionDefaults();

	m_isScopedProfileSection = isScopedProfileSection;
	m_profileCategory = profileCategory;

	if ( m_isScopedProfileSection ) {

		TimeSystem& sharedTimeSystem = TimeSystem::getSharedTimeSystem();
		m_startTimeSeconds = sharedTimeSystem.getCurrentTimeSeconds();
	}
}


void ProfileSection::start() {

	TimeSystem& sharedTimeSystem = TimeSystem::getSharedTimeSystem();
	m_startTimeSeconds = sharedTimeSystem.getCurrentTimeSeconds();
}


void ProfileSection::stop() {

	TimeSystem& sharedTimeSystem = TimeSystem::getSharedTimeSystem();
	m_endTimeSeconds = sharedTimeSystem.getCurrentTimeSeconds();

	m_durationOfProfileForFrameSeconds = static_cast<float>( m_endTimeSeconds - m_startTimeSeconds );

	EngineProfiler& engineProfiler = EngineProfiler::getSharedEngineProfiler();
	engineProfiler.reportProfileSectionResult( *(this) );

	// Prevent accidental double dipping
	if ( m_isScopedProfileSection ) {
		m_isScopedProfileSection = false;
	}

	m_startTimeSeconds = 0.0;
	m_endTimeSeconds = 0.0;
	m_durationOfProfileForFrameSeconds = 0.0f;
}


void ProfileSection::setProfileSectionDefaults() {

	m_startTimeSeconds = 0.0;
	m_endTimeSeconds = 0.0;

	m_profileCategory = PROFILE_CATEGORY_NONE;
	m_isScopedProfileSection = false;
}