#ifndef included_TypedNamedProperty
#define included_TypedNamedProperty
#pragma once

#include <string>

#include "NamedProperty.hpp"

template < typename T_PropertyDataType >
class TypedNamedProperty : public NamedProperty {
public:
	explicit TypedNamedProperty<T_PropertyDataType>() {}
	virtual ~TypedNamedProperty() {}

	// Inline Mutators
	inline void TypedNamedProperty<T_PropertyDataType>::setData( const T_PropertyDataType & data ) {
		m_data = data;
	}


	inline void TypedNamedProperty<T_PropertyDataType>::getData( T_PropertyDataType & data ) {
		data = m_data;
	}

protected:

private:
	T_PropertyDataType		m_data;

};


#endif