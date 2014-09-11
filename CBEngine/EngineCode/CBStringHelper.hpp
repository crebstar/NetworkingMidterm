#ifndef included_CBStringHelper
#define included_CBStringHelper
#pragma once

#include <vector>
#include <string>
#include <cstdlib>
#include <algorithm>

namespace cbengine {

extern void convertDelimitedStringToVectorOfStrings( const std::string & delimitedString, char delimiter, std::vector< std::string > & stringVec );

// Inline Functions
float convertStringToFloat( const std::string& stringToConvert );
int convertStringToInt( const std::string& stringToConvert );
/*
	Returns true if the conversion was successful
*/
bool convertStringToBool( const std::string& stringToConvert, bool& boolConversion );

} // end namespace


inline float cbengine::convertStringToFloat( const std::string& stringToConvert ) {
	float conversion = static_cast<float>(std::strtod( stringToConvert.c_str(), 0 ) );
	return conversion;
}


inline int cbengine::convertStringToInt( const std::string& stringToConvert ) {

	int conversion = atoi( stringToConvert.c_str() ); 
	return conversion;
}


inline bool cbengine::convertStringToBool( const std::string& stringToConvert, bool& boolConversion ) {
	if ( stringToConvert.length() > 5 || stringToConvert.length() < 4 ) {
		return false;
	}
	
	std::string conversionCopy(stringToConvert);
	std::transform( conversionCopy.begin(), conversionCopy.end(), conversionCopy.begin(), ::tolower );
	if ( conversionCopy[0] == 't' ) {
		if ( conversionCopy[1] == 'r' && conversionCopy[2] == 'u' && conversionCopy[3] == 'e' ) {
			boolConversion = true;
			return true;
		}
	}

	if ( conversionCopy[0] == 'f' ) {
		if( conversionCopy[1] == 'a' && conversionCopy[2] == 'l' && conversionCopy[3] == 's' && conversionCopy[4] == 'e' ) {
			boolConversion = false;
			return true;
		}
	}
	
	return false;
}

#endif