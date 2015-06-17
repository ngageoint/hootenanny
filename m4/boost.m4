AC_DEFUN([AX_LIB_BOOST],
[
    AC_LANG_PUSH([C++])
    AC_CHECK_HEADERS([boost/property_map.hpp boost/property_map/property_map.hpp], [hootFoundBoostPropHeaders=yes; break;])
    AS_IF([test "x$hootFoundBoostPropHeaders" != "xyes"], [AC_MSG_ERROR([Unable to find the boost property_map headers])])
    AC_LANG_POP
    
    # This is an ugly way to look for a c++ function but it works.
    # Boost on Ubuntu 1404 needs the "boost_system" library
    AC_SEARCH_LIBS([_ZN5boost6system15system_categoryEv], [boost_system], [])
    # Support for zlib
    AC_SEARCH_LIBS([_ZN5boost9iostreams4zlib10sync_flushE], [boost_iostreams], [])
])

