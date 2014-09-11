#include "CBStringHelper.hpp"

#include <cassert>

#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

void cbengine::convertDelimitedStringToVectorOfStrings( const std::string & delimitedString, char delimiter, std::vector< std::string > & stringVec ) {
	if ( delimitedString.length() == 0 ) {
		return;
	}

	std::string stringToAdd("");
	for ( size_t i = 0; i < delimitedString.length(); ++i ) {
		if ( delimitedString[i] == delimiter ) {
			stringVec.push_back( stringToAdd );
			stringToAdd.clear();
		} else {
			stringToAdd += delimitedString[i];
		}
	}
	// Put remaining contents in the vector
	stringVec.push_back( stringToAdd );
}

