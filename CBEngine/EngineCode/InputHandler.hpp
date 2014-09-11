#ifndef included_InputHandler
#define included_InputHandler
#pragma once

#include <vector>
#include <queue>
#include <unordered_map>
#include <cassert>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "EventSystemSubscriber.hpp"
#include "TypedEventSubscriber.hpp"
#include "NamedProperties.hpp"
#include "NamedProperty.hpp"


const int NUM_INPUT_VIRTUAL_KEYS = 256;

// Input Function
// TODO :: Remove dependency on windows input and move this to a different file
LRESULT CALLBACK windowsMessageHandlingProcedure( HWND windowHandle,
	UINT wmMessageCode, 
	WPARAM wParam, 
	LPARAM lParam );

class InputHandler {
public:

	static InputHandler& getSharedInputHandler() {

		static InputHandler inputHandler;
		return inputHandler;
	}

	~InputHandler() {}

	template< typename T_RegisteringObjectType, typename T_ObjectMethodType >
	void addInputSubscriberForInputEvent( unsigned char inputEventToListenFor, T_RegisteringObjectType* classObject, T_ObjectMethodType callbackFunction ) {

		assert( classObject != nullptr );

		TypedEventSubscriber<T_RegisteringObjectType>* inputEventSubscriber = new TypedEventSubscriber<T_RegisteringObjectType>;
		inputEventSubscriber->setSubscriber( classObject );
		inputEventSubscriber->setCallbackFunction( callbackFunction );

		std::unordered_map<unsigned char,std::vector<EventSubscriberBase*>>::iterator itInputSub;
		itInputSub = m_inputEventSubscribers.find( inputEventToListenFor );

		if ( itInputSub != m_inputEventSubscribers.end() ) {

			std::vector<EventSubscriberBase*>& eventSubVec = itInputSub->second;
			eventSubVec.push_back( inputEventSubscriber );

		} else {
			// Event not previously registered for
			std::vector<EventSubscriberBase*> eventSubVec;
			eventSubVec.push_back( inputEventSubscriber );

			std::pair<unsigned char,std::vector<EventSubscriberBase*>> elementToInsert;
			elementToInsert.first = inputEventToListenFor;
			elementToInsert.second = eventSubVec;
			m_inputEventSubscribers.insert( elementToInsert );

		}
	}


	template< typename T_RegisteringObjectType >
	void unregisterInputEventCallbackForObject( unsigned char inputEvent, T_RegisteringObjectType* classObject ) {

		std::unordered_map<unsigned char,std::vector<EventSubscriberBase*>>::iterator itInput;
		itInput = m_inputEventSubscribers.find( inputEvent );

		if ( itInput != m_inputEventSubscribers.end() ) {

			std::vector<EventSubscriberBase*>& subVector = itInput->second;
			size_t indexValueToRemoveElement = 0;
			bool matchFound = false;

			for ( size_t i = 0; i < subVector.size(); ++i ) {

				EventSubscriberBase* subscriberBase = subVector[i];
				TypedEventSubscriber<T_RegisteringObjectType>* subscriberTyped = dynamic_cast<TypedEventSubscriber<T_RegisteringObjectType*>( subscriberBase );

				if ( subscriberTyped != nullptr ) {
					// Match Found
					T_RegisteringObjectType* subscriberObject = subscriberTyped->getSubscriber();
					if ( classObject == subscriberObject ) {
						// Object Match
						indexValueToRemoveElement = i;
						matchFound = true;
					}
				}
			} // end for

			if ( matchFound ) {

				delete subVector[indexValueToRemoveElement];
				subVector.erase( subVector.begin() + indexValueToRemoveElement );
			}
		}
	}

	// Input
	bool							processKeyDownEvent( unsigned char virtualKeyCode );
	bool							processKeyUpEvent( unsigned char virtualKeyCode );
	void							processWMCHARKeyEvent( unsigned char wmCharKey );
	void							emptyWMCharMessageQueue();

	const bool *					getKeyboardStatus() const;
	int								getNumVirtualKeys() const;
	std::queue<unsigned char>&		getWmCharMessageQueue();

	bool callCallbackFunctionForInputEventSubscribers( unsigned char inputEventKey );

	// TODO:: Make this more platform agnostic
	void registerWindowsInputCallbackFunction( WNDCLASSEX& windowClassDescription );

protected:

	InputHandler() {

		setDefaultValuesForInputKeys();
	}

	void setDefaultValuesForInputKeys();
private:

	std::unordered_map<unsigned char,std::vector<EventSubscriberBase*>>			m_inputEventSubscribers;
	bool																		m_isKeyDown[NUM_INPUT_VIRTUAL_KEYS]; // Temp
	std::queue<unsigned char>													m_wmCharKeys;

	//const NamedProperties														EMPTY_NAMED_PROPERTIES;
};


#endif