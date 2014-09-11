#include "EventSystemSubscriber.hpp"

#include "EventSystem.hpp"

#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

EventSystemSubscriber::EventSystemSubscriber() {
	
}
EventSystemSubscriber::~EventSystemSubscriber() {
	//cbengine::UnregisterObjectForAllEvents( this );
	EventSystem & sharedEventSystem = EventSystem::getSharedEventSystem();
	sharedEventSystem.unregisterEventSystemSubscriber( this );

}