#ifndef included_NamedProperties
#define included_NamedProperties
#pragma once

#include <map>
#include <string>

#include "NamedProperty.hpp"
#include "TypedNamedProperty.hpp"

typedef enum {

	PROPERTY_GET_SUCCESS,
	PROPERTY_GET_FAILED_NO_SUCH_PROPERTY,
	PROPERTY_GET_FAILED_WRONG_TYPE,
	PROPERTY_GET_FAILED_NULLPTR,

} PropertyResult;

	
class NamedProperties {
public:
	~NamedProperties();

	// Mutators
	template <typename T_PropertyDataType>
	inline PropertyResult getProperty( const std::string & propertyName, T_PropertyDataType & valueOut ) {

		PropertyResult result;
		std::map<std::string, NamedProperty *>::iterator itProp;
		itProp = m_namedProperties.find( propertyName );

		if ( itProp != m_namedProperties.end() ) {
			// property found in the map... Need to cast it to check if the type is correct
			NamedProperty * retrievedResult = itProp->second;
			if ( retrievedResult == nullptr ) {
				result = PROPERTY_GET_FAILED_NULLPTR;
				return result;
			} 

			TypedNamedProperty<T_PropertyDataType> * propertyWithType = dynamic_cast<TypedNamedProperty<T_PropertyDataType> *>( retrievedResult );

			if ( propertyWithType != nullptr ) {

				propertyWithType->getData( valueOut );
				result = PROPERTY_GET_SUCCESS;

			} else {

				result = PROPERTY_GET_FAILED_WRONG_TYPE;
			}

		} else {
			// No such property exists in the map
			result = PROPERTY_GET_FAILED_NO_SUCH_PROPERTY;
		
		}

		return result;
	} // end getProperty

	// TODO:: handle cases where a duplicate property is given
	template <typename T_PropertyDataType>
	inline void setProperty( const std::string & propertyName, const T_PropertyDataType & propertyValue ) {

		TypedNamedProperty<T_PropertyDataType> * typedProperty = new TypedNamedProperty<T_PropertyDataType>;
		typedProperty->setData( propertyValue );
		m_namedProperties.insert( std::pair< std::string, TypedNamedProperty<T_PropertyDataType>*>( propertyName, typedProperty ) );
	}


	inline void setProperty( const std::string & propertyName, const char * propertyValue ) {

		std::string propertyValueAsString( propertyValue );
		TypedNamedProperty<std::string> * typedProperty = new TypedNamedProperty<std::string>;
		typedProperty->setData( propertyValueAsString );
		m_namedProperties.insert(std::pair<std::string,TypedNamedProperty<std::string>*>( propertyName, typedProperty ));
	}

protected:

private:
	std::map<std::string, NamedProperty* >			m_namedProperties;

};


#endif
