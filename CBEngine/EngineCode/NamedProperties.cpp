#include "NamedProperties.hpp"

#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

NamedProperties::~NamedProperties() {

	std::map<std::string, NamedProperty* >::iterator it;

	for ( it = m_namedProperties.begin(); it != m_namedProperties.end(); ++it ) {

		NamedProperty* propToDelete = it->second;
		delete propToDelete;
	}
}