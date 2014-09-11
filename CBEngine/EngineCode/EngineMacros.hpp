


#ifndef included_EngineMacros
#define included_EngineMacros
#pragma once

#define PREVENT_COPY_AND_ASSIGN(TypeName) \
	private: \
	TypeName( const TypeName & t ); \
	void operator = ( const TypeName & t );
	
#define UNUSED(x) (void)(x);

#define STATIC 

#endif