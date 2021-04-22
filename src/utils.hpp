#ifndef UTILS_H
#define UTILS_H

#include <sstream>
#ifdef SSTR
#undef SSTR
#endif

#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

#endif