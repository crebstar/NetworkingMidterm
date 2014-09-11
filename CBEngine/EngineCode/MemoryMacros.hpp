#ifndef included_MemoryMacros
#define included_MemoryMacros
#pragma once

#ifdef USE_MEMORY_MANAGER

#include "OperatorNewDelete.hpp"
#define new new(__FILE__,__LINE__)

#endif

#endif