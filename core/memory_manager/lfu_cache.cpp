#include "lfu_cache.h"
// Includes the LFUCache header which contains the complete template class definition and all method bodies.
// Because LFUCache is a class template, the compiler needs the full implementation at every point
// of instantiation — so all method bodies live in the header, not here.

// Template implementations are in the header.
// This note explains why the .cpp file is nearly empty: C++ templates cannot be separately compiled
// and then linked; the full template definition must be visible wherever the template is instantiated.
