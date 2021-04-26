#ifndef UTILS_H
#define UTILS_H

#define BUILD_INFO  "v0.3 " __DATE__
//#define DEBUG

#include <sstream>
#ifdef SSTR
#undef SSTR
#endif

#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

typedef enum{
    nothing=0,
    N=1U, S=2U, E=4U, W=8U,
} t_direction;

#endif
