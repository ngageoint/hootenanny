AC_DEFUN([LIBPQ_INIT],[
  AC_SUBST(HAS_LIBPQ) 

  CPPFLAGS="-I/usr/include/postgresql ${CPPFLAGS}"
  LIBS="-lpq ${LIBS}"

  #AC_CHECK_LIB([libpq], [main], [], AC_MSG_FAILURE("liqpq is required."))

  AC_CHECK_HEADERS([postgresql/libpq-fe.h], [hootFoundLibpqHeaders=yes; break;])
  AS_IF([test "x$hootFoundLibpqHeaders" != "xyes"], [AC_MSG_ERROR([Unable to find libpq-fe.h])])

])
