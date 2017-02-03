# hoot-nodejs provides web services for hootenanny
AC_DEFUN([NODEJS_INIT],[
    AC_SUBST(HAS_NODEJS, "nodejs")

    # Now find out which version we have
    # We set the include path in the hoot-js.pro file 
    saved_CPPFLAGS=$CPPFLAGS
    node_Flags=""

    AC_LANG_PUSH([C++])
    CPPFLAGS="-I/usr/include/nodejs/src -I/usr/include/nodejs/deps/uv/include -I/usr/include/nodejs/deps/v8/include ${saved_CPPFLAGS}"
    AC_CHECK_HEADERS([nodejs/src/node.h], [hootFoundNjsHeaders=yes; node_Flags="-I/usr/include/nodejs -I/usr/include/nodejs/deps/uv/include -I/usr/include/nodejs/deps/v8/include"; break;])

    CPPFLAGS="-I/usr/include/nodejs ${saved_CPPFLAGS}"
    AC_CHECK_HEADERS([nodejs/node.h], [hootFoundNjsHeaders=yes; node_Flags="-I/usr/include/nodejs"; break;])

    CPPFLAGS="-I/usr/include/node ${saved_CPPFLAGS}"
    AC_CHECK_HEADERS([node/node.h], [hootFoundNjsHeaders=yes;  node_Flags="-I/usr/include/node"; break;],[],[#include <v8.h>])

    AC_LANG_POP

    CPPFLAGS=$saved_CPPFLAGS

    #AS_IF([test "x$hootFoundNjsHeaders" != "xyes"], [AC_MSG_ERROR([Unable to find the NodeJs headers])])
    AS_IF([test "x$hootFoundNjsHeaders" = "xyes"], [CPPFLAGS="${node_Flags} ${CPPFLAGS}"], [AC_MSG_ERROR([Unable to find the NodeJs headers])])
    AC_DEFINE([HAVE_NODEJS], [1], [node.js is present])
])
