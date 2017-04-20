AC_DEFUN([LIBPQ_INIT],[
  AC_SUBST(HAS_LIBPQ) 

  CPPFLAGS="-I/usr/include/postgresql -I/usr/pgsql-9.2/include ${CPPFLAGS}"
  LIBS="-lpq ${LIBS}"

  #AC_CHECK_LIB([libpq], [main], [], AC_MSG_FAILURE("liqpq is required."))

  # pg 9.5 on ubuntu 14.04
  AC_CHECK_HEADERS([postgresql/libpq-fe.h], [hootFoundLibpqHeaders=yes; break;])
  # pg 9.2 on centos 6.7 (PR job; patty)
  AC_CHECK_HEADERS([libpq-fe.h], [hootFoundLibpqHeaders=yes; break;])
  # pg 9.2 on centos 6.7 (RPM job)
  AC_CHECK_HEADERS([/usr/pgsql-9.2/include/libpq-fe.h], [hootFoundLibpqHeaders=yes; break;])
  AS_IF([test "x$hootFoundLibpqHeaders" != "xyes"], [AC_MSG_ERROR([Unable to find libpq-fe.h])])

])
