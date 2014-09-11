#ifndef included_PromptDelegate
#define included_PromptDelegate
#pragma once

#include <string>

#include "Vector4.hpp"

class PromptDelegate {
public:

	virtual void addStringToConsole( const std::string & stringToAdd, const cbengine::Vector4<float> & color ) = 0;
	virtual void clearCommandHistory() = 0;

protected:

private:

};


#endif