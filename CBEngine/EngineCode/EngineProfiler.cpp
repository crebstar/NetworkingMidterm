#include "EngineProfiler.hpp"

#include "Console.hpp"
#include "ProfileSection.hpp"
#include "CBStringHelper.hpp"

#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

EngineProfiler::~EngineProfiler() {

}


void EngineProfiler::onEndOfFrame() {

	Console* sharedDeveloperConsole = Console::getSharedDeveloperConsole();

	for ( size_t i = 0; i < NUM_PROFILE_CATEGORIES; ++i ) {

		ProfileResult& profileResult = m_profileResults[i];

		if ( profileResult.m_durationMeasuredCurrentFrame > 0.0f ) {

			profileResult.m_numFramesMeasured++;
			profileResult.m_totalDurationMeasured += profileResult.m_durationMeasuredCurrentFrame;

			if ( m_shouldReportCurrentProfileResultsAtEndOfFrame ) {

				std::string consoleLineText = getStringForEnumValue( profileResult.m_profileCategory );

				consoleLineText += " - Duration Current Frame: ";
				std::string durationThisFrameAsString = std::to_string( static_cast<long double>( profileResult.m_durationMeasuredCurrentFrame ) ); 
				consoleLineText += durationThisFrameAsString;

				consoleLineText += " Average Time Per Frame: ";
				float averageTimePerFrame = profileResult.m_totalDurationMeasured / static_cast<float>( profileResult.m_numFramesMeasured );
				std::string averageTimePerFrameAsString = std::to_string( static_cast<long double>( averageTimePerFrame ) );
				consoleLineText += averageTimePerFrameAsString;

				sharedDeveloperConsole->addStringToConsole( consoleLineText, ConsoleTextColorForProfiler );
			}
		}

		profileResult.m_durationMeasuredCurrentFrame = 0.0f;
	}

	m_shouldReportCurrentProfileResultsAtEndOfFrame = false;
}


void EngineProfiler::reportProfileSectionResult( const ProfileSection& profileSection ) {

	ProfileCategory sectionCategory = profileSection.m_profileCategory;
	ProfileResult& profResult = m_profileResults[ sectionCategory ];

	profResult.m_durationMeasuredCurrentFrame += profileSection.m_durationOfProfileForFrameSeconds;
}


void EngineProfiler::clearCurrentFrameDurationsForProfileResults() {

	for ( size_t i = 0; i < NUM_PROFILE_CATEGORIES; ++i ) {

		ProfileResult& profileResult = m_profileResults[i];
		profileResult.m_durationMeasuredCurrentFrame = 0.0f;
	}
}


void EngineProfiler::reportProfileResultsAtEndOfFrame() {

	m_shouldReportCurrentProfileResultsAtEndOfFrame = true;
}


void EngineProfiler::setEngineProfilerDefaults() {


	for ( size_t i = 0; i < NUM_PROFILE_CATEGORIES; ++i ) {

		ProfileCategory profCat = static_cast<ProfileCategory>( i );
		ProfileResult& profileResult = m_profileResults[i];
		profileResult.m_profileCategory = profCat;
	}

	m_shouldReportCurrentProfileResultsAtEndOfFrame = false;
}