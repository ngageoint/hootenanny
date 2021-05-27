# hoot-nodejs provides web services for hootenanny
AC_DEFUN([NODEJS_INIT],[
  AC_SUBST(HAS_NODEJS, "nodejs")

  # Now find out which version we have
  # We set the include path in the hoot-js.pro file 
  #saved_CPPFLAGS=$CPPFLAGS
  CPPFLAGS="-std=c++11 -I/usr/include -I/usr/include/nodejs -I/usr/include/node -I/usr/include/nodejs/deps/uv/include ${CPPFLAGS}"

  AC_LANG_PUSH([C++])
    AC_CHECK_HEADERS([nodejs/src/node.h], [hootFoundNjsHeaders=yes; break;])
    AC_CHECK_HEADERS([nodejs/node.h], [hootFoundNjsHeaders=yes; break;])
    AC_CHECK_HEADERS([node/node.h], [hootFoundNjsHeaders=yes; break;])
  AC_LANG_POP

  AS_IF([test "x$hootFoundNjsHeaders" != "xyes"], [AC_MSG_ERROR([Unable to find the NodeJs headers])])
  AC_DEFINE([HAVE_NODEJS], [1], [node.js is present])
])
