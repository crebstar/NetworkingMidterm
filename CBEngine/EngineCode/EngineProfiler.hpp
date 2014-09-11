#ifndef included_EngineProfiler
#define included_EngineProfiler
#pragma once

#include <vector>

#include "EngineMacros.hpp"
#include "Vector4.hpp"

typedef enum {

	PROFILE_CATEGORY_NONE = 0,
	PROFILE_CATEGORY_UPDATE,
	PROFILE_CATEGORY_RENDER,
	PROFILE_CATEGORY_ANIMATION,
	PROFILE_CATEGORY_GAMEPLAY,
	PROFILE_CATEGORY_AI,
	PROFILE_CATEGORY_PHYSICS,
	PROFILE_CATEGORY_INPUT,
	PROFILE_CATEGORY_MISC,
	NUM_PROFILE_CATEGORIES,
} ProfileCategory;


const std::string ProfileCategoryStrings[] = {
	"None",
	"Update",
	"Render",
	"Animation",
	"Gameplay",
	"AI",
	"Physics",
	"Input",
	"Misc",
	"Number Categories"
};


struct ProfileResult {
public:
	ProfileResult() {
		setProfileResultDefaults();
	}

	ProfileCategory   m_profileCategory;
	float			  m_durationMeasuredCurrentFrame;
	float			  m_totalDurationMeasured;
	unsigned int	  m_numFramesMeasured;

protected:
	void setProfileResultDefaults() {

		m_profileCategory = PROFILE_CATEGORY_NONE;
		m_durationMeasuredCurrentFrame = 0.0f;
		m_numFramesMeasured = 0;
	}
};

const cbengine::Vector4<float> ConsoleTextColorForProfiler( 1.0f, 1.0f, 0.90f, 1.0f );

class ProfileSection;

class EngineProfiler {
public:
	static EngineProfiler& getSharedEngineProfiler() {

		static EngineProfiler engineProfiler;
		return engineProfiler;
	}

	~EngineProfiler();

	void reportProfileSectionResult( const ProfileSection& profileSection );
	void onEndOfFrame();

	void reportProfileResultsAtEndOfFrame();

	const std::string getStringForEnumValue( ProfileCategory profCat ) const;
protected:
	EngineProfiler() {
		setEngineProfilerDefaults();
	}

	void clearCurrentFrameDurationsForProfileResults();
	void outputCurrentProfileResultsToConsole();

	void setEngineProfilerDefaults();
private:
	PREVENT_COPY_AND_ASSIGN( EngineProfiler );

	ProfileResult			m_profileResults[ NUM_PROFILE_CATEGORIES ];

	bool					m_shouldReportCurrentProfileResultsAtEndOfFrame;
};

// Inline Functions
inline const std::string EngineProfiler::getStringForEnumValue( ProfileCategory profCat ) const {

	return ProfileCategoryStrings[profCat];
}

#endif