#ifndef included_EventSubscriberBase
#define included_EventSubscriberBase
#pragma once

#include "NamedProperties.hpp"

class EventSubscriberBase {
public:
	virtual ~EventSubscriberBase() {}
	EventSubscriberBase() {}

	virtual void callCallbackFunction( NamedProperties & params ) = 0;
	
	virtual void * getSubscriberObjectPointer() = 0;

protected:

private:

};


#endif