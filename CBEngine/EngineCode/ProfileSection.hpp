#ifndef included_ProfileSection
#define included_ProfileSection
#pragma once

#include "EngineMacros.hpp"
#include "EngineProfiler.hpp"

class ProfileSection {
public:
	friend class EngineProfiler;

	~ProfileSection();
	explicit ProfileSection( ProfileCategory profileCategory, bool isScopedProfileSection );

	void start();
	void stop();

protected:

	void setProfileSectionDefaults();
private:
	PREVENT_COPY_AND_ASSIGN( ProfileSection );

	ProfileCategory			m_profileCategory;
	double					m_startTimeSeconds;
	double					m_endTimeSeconds;
	float					m_durationOfProfileForFrameSeconds;

	bool					m_isScopedProfileSection;
};

#endif