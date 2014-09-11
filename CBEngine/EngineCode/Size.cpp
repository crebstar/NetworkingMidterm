
#include "Size.hpp"

#include "../../CBEngine/EngineCode/MemoryMacros.hpp"


cbengine::Size::~Size() {


} // end dtor

cbengine::Size::Size( float width /* = 0.0f */, float height /* = 0.0f */ )  :
	m_width(width),
	m_height(height)
{


} // end Ctor

bool cbengine::Size::sizeEqualsSize( const Size & sizeToCompare ) const {

	return ( (m_width == sizeToCompare.m_width) && (m_height == sizeToCompare.m_height) );

} // end sizeEqualsSize