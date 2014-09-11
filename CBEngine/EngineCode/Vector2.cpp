
#include "Vector2.hpp"

#include <math.h>

#include "CBStringHelper.hpp"

#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

cbengine::Vector2::~Vector2() {


} // end destructor

cbengine::Vector2::Vector2(float x /* = 0.0f */, float y /* = 0.0f */) : 
	x(x),
	y(y)
{

} // end Ctor


cbengine::Vector2::Vector2( const std::string& vecString ) {
	int numberIndex = 0;
	std::string currentString;
	for ( size_t i = 0; i < vecString.size(); ++i ) {
		if ( vecString[i] == '(' ) {
			continue;
		}
		if ( vecString[i] == ',' ) {
			++numberIndex;
			if ( numberIndex == 1 ) {
				x = cbengine::convertStringToFloat( currentString );
				currentString.clear();
				++i;
			} 
		}
		currentString += vecString[i];
	}
	y = cbengine::convertStringToFloat( currentString );
}


void cbengine::Vector2::normalizeVector() {

	float lengthOfVector = sqrt( (x * x) + (y * y) );

	if ( lengthOfVector == 0.0f ) {
		return;
	}

	x = x / lengthOfVector;
	y = y / lengthOfVector;

}


