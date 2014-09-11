#ifndef included_TypedEventSubscriber
#define included_TypedEventSubscriber
#pragma once

#include <cassert>

#include "EventSubscriberBase.hpp"
// void callCallbackFunction( NamedProperties & params )
template< typename T_SubscriberType >
class TypedEventSubscriber : public EventSubscriberBase {

typedef void ( T_SubscriberType::*ObjectMemberFunctionType )( NamedProperties & );

public:
	
	virtual ~TypedEventSubscriber<T_SubscriberType>() {}
	explicit TypedEventSubscriber<T_SubscriberType>() {
		m_subscriberObject = nullptr;
	}

	virtual void callCallbackFunction( NamedProperties & params ) {
		(m_subscriberObject->*m_callbackFunction)(params);
	}


	void setCallbackFunction( ObjectMemberFunctionType callbackFunction ) {
		m_callbackFunction = callbackFunction;
	}


	void setSubscriber( T_SubscriberType * subscriber ) {
		assert( subscriber != nullptr );
		m_subscriberObject = subscriber;
	}


	T_SubscriberType * getSubscriber() {
		return m_subscriberObject;
	}

	virtual void * getSubscriberObjectPointer() {
		return m_subscriberObject;
	}

protected:

private:

	T_SubscriberType *			m_subscriberObject;
	ObjectMemberFunctionType    m_callbackFunction;

};



#endif