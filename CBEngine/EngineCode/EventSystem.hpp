#ifndef included_EventSystem
#define included_EventSystem
#pragma once

#include <map>
#include <vector>
#include <string>
#include <cassert>

#include "EventSystemSubscriber.hpp"
#include "EventSubscriberBase.hpp"
#include "TypedEventSubscriber.hpp"

class EventSystem {
public:

	static EventSystem & getSharedEventSystem() {
		static EventSystem eventSystem;
		return eventSystem;
	}

	~EventSystem();

	// TODO :: Handle insertion failure/duplicates
	template< typename T_RegisteringObjectType, typename T_ObjectMethodType >
	void registerEvent( const std::string & eventName, T_RegisteringObjectType * classObject, T_ObjectMethodType callbackFunction ) {
		assert( classObject != nullptr );
		TypedEventSubscriber< T_RegisteringObjectType > * eventSubscriber = new TypedEventSubscriber< T_RegisteringObjectType >;
		eventSubscriber->setSubscriber( classObject );
		eventSubscriber->setCallbackFunction( callbackFunction );

		std::map< std::string, std::vector<EventSubscriberBase*>>::iterator it;
		it = m_eventSubscribers.find( eventName );
		if ( it != m_eventSubscribers.end() ) {
			std::vector<EventSubscriberBase*>& eventSubVec = it->second;
			eventSubVec.push_back( eventSubscriber );
		} else {
			// Event did not previously exist
			std::vector<EventSubscriberBase*> eventSubVec;
			eventSubVec.push_back( eventSubscriber );
			std::pair<std::string,std::vector<EventSubscriberBase*>> elementToInsert;
			elementToInsert.first = eventName;
			elementToInsert.second = eventSubVec;
			m_eventSubscribers.insert(elementToInsert);
		}
	}


	void fireEvent( const std::string & eventName, NamedProperties & params ) {

		std::map<std::string,std::vector<EventSubscriberBase*>>::iterator itFind;
		itFind = m_eventSubscribers.find( eventName );
		if ( itFind != m_eventSubscribers.end() ) {
			// Someone has registered for this event
			std::vector<EventSubscriberBase*> & subscribersVector = itFind->second;
			for ( size_t i = 0; i < subscribersVector.size(); ++i ) {
				EventSubscriberBase * eventSubscriber = subscribersVector[i];
				eventSubscriber->callCallbackFunction( params );
			}
		}
	}

	// For if you want to fire an event with no params given
	void fireEvent( const std::string & eventName ) {
		NamedProperties blankParams;
		fireEvent( eventName, blankParams );
	}


	template< typename T_RegisteringObjectType >
	void unregisterEventCallbackForObject( const std::string & eventName, T_RegisteringObjectType * classObject ) {

		std::map<std::string,std::vector<EventSubscriberBase*>>::iterator it;
		it = m_eventSubscribers.find( eventName );

		if ( it != m_eventSubscribers.end() ) {

			std::vector<EventSubscriberBase*> & subVector = it->second;
			size_t indexValueToRemoveElement = 0;
			bool matchFound = false;

			for ( size_t i = 0; i < subVector.size(); ++i ) {
				EventSubscriberBase * subscriberBase = subVector[i];
				TypedEventSubscriber<T_RegisteringObjectType> * subscriberTyped = dynamic_cast<TypedEventSubscriber<T_RegisteringObjectType>*>( subscriberBase );
				if ( subscriberTyped != nullptr ) {
					// The types match... now just need to check if the objects point to the same memory location
					T_RegisteringObjectType * subscriberObject = subscriberTyped->getSubscriber();
					if ( classObject == subscriberObject ) {
						// At this point we know this is the exact object which registered for the event
						indexValueToRemoveElement = i;
						matchFound = true;
						// Can break out of loop since a subscriber can only subscribe to an event once
						break;
					}
				}
			} // end for

			if ( matchFound ) {
				// I think this is correct way to manage memory here
				delete subVector[indexValueToRemoveElement];
				subVector.erase( subVector.begin() + indexValueToRemoveElement );
			} // end inner if
		} // end outer if
	} // end function


	template< typename T_RegisteringObjectType >
	void unregisterObjectForAllEvents( T_RegisteringObjectType * classObject ) {
		std::map<std::string,std::vector<EventSubscriberBase*>>::iterator it;
		for ( it = m_eventSubscribers.begin(); it != m_eventSubscribers.end(); ++it ) {
			std::vector<EventSubscriberBase*> & subVector = it->second;
			bool matchFound = false;
			size_t indexValueToRemoveElement = 0;
			for( size_t i = 0; i < subVector.size(); ++i ) {
				EventSubscriberBase * subscriberBase = subVector[i];
				TypedEventSubscriber<T_RegisteringObjectType> * subscriberTyped = dynamic_cast<TypedEventSubscriber<T_RegisteringObjectType>*>( subscriberBase );
				if ( subscriberTyped != nullptr ) {
					// The types match... now just need to check if the objects point to the same memory location
					T_RegisteringObjectType * subscriberObject = subscriberTyped->getSubscriber();
					if ( classObject == subscriberObject ) {
						// At this point we know this is the exact object which registered for the event
						indexValueToRemoveElement = i;
						matchFound = true;
						// Can break out of loop since a subscriber can only subscribe to an event once
						break;
					}
				}
			} // end inner for
			if ( matchFound ) {
				// I think this is correct way to manage memory here
				delete subVector[indexValueToRemoveElement];
				subVector.erase( subVector.begin() + indexValueToRemoveElement );
			} // end if
		} // end outer for
	} // end function


	void unregisterEventSystemSubscriber( EventSystemSubscriber * subscriber ) {
		std::map<std::string,std::vector<EventSubscriberBase*>>::iterator it;
		for ( it=m_eventSubscribers.begin(); it != m_eventSubscribers.end(); ++it ) {
			std::vector<EventSubscriberBase*> & eventSubVector = it->second;
			size_t indexOfElementToRemove = 0;
			bool matchFound = false;
			for ( size_t i = 0; i < eventSubVector.size(); ++i ) {
				EventSubscriberBase * subscriberWithCallback = eventSubVector[i];
				void * objectWithCallback = subscriberWithCallback->getSubscriberObjectPointer();
				if ( objectWithCallback == subscriber ) {
					matchFound = true;
					indexOfElementToRemove = i;
					break;
				}
			} // end for
			if ( matchFound ) {
				delete eventSubVector[indexOfElementToRemove];
				eventSubVector.erase( eventSubVector.begin() + indexOfElementToRemove );
			}
			
		} // end for

	} // end function

protected:
	// Singleton protected constructor
	EventSystem() {}

private:
	std::map< std::string, std::vector<EventSubscriberBase *>>			m_eventSubscribers;
	
};

// Below is a collection of convenience functions if one wishes to circumvent the singleton for registry
namespace cbengine {

	template< typename T_RegisteringObjectType, typename T_ObjectMethodType >
	inline void RegisterEventCallbackForObject( const std::string & eventName, T_RegisteringObjectType * classObject, T_ObjectMethodType callbackFunction ) {
		EventSystem & sharedEventSystem = EventSystem::getSharedEventSystem();
		sharedEventSystem.registerEvent( eventName, classObject, callbackFunction );

	}


	inline void FireEvent( const std::string & eventName, NamedProperties & params ) {
		EventSystem & sharedEventSystem = EventSystem::getSharedEventSystem();
		sharedEventSystem.fireEvent( eventName, params );
	}


	inline void FireEvent( const std::string & eventName ) {
		NamedProperties blankParams;
		EventSystem & sharedEventSystem = EventSystem::getSharedEventSystem();
		sharedEventSystem.fireEvent( eventName, blankParams );
	}


	template< typename T_RegisteringObjectType >
	inline void UnregisterEventCallbackForObject( const std::string & eventName, T_RegisteringObjectType * classObject ) {
		EventSystem & sharedEventSystem = EventSystem::getSharedEventSystem();
		sharedEventSystem.unregisterEventCallbackForObject( eventName, classObject );
	}


	template< typename T_RegisteringObjectType >
	inline void UnregisterObjectForAllEvents( T_RegisteringObjectType * classObject ) {
		EventSystem & sharedEventSystem = EventSystem::getSharedEventSystem();
		sharedEventSystem.unregisterObjectForAllEvents( classObject );
	}

}


#endif