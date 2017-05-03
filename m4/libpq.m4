AC_DEFUN([LIBPQ_INIT],[
  AC_SUBST(HAS_LIBPQ) 

  CPPFLAGS="-I/usr/include/postgresql -I/usr/pgsql-9.2/include ${CPPFLAGS}"
  LIBS="-lpq -L/usr/pgsql-9.2/lib ${LIBS}"

  # pg 9.5 on ubuntu 14.04
  # /usr/lib/x86_64-linux-gnu - picked up automatically
  # pg 9.2 on centos 6.7 (PR job; patty)
  # /usr/lib - picked up automatically
  # pg 9.2 on centos 6.7 (RPM job)
  # /usr/pgsql-9.2/lib
  # hoot uses this one
  AC_CHECK_LIB(pq, PQconnectdb, [], AC_MSG_ERROR(libpq not found))
  # this one guarantees us pg 9.x
  AC_CHECK_LIB(pq, PQconnectdbParams, [], AC_MSG_ERROR(unsupported version of libpq))

  #  (header-file, [action-if-found], [action-if-not-found], [includes])
  # pg 9.5 on ubuntu 14.04
  AC_CHECK_HEADERS([postgresql/libpq-fe.h], [hootFoundLibpqHeaders=yes; break;])
  # pg 9.2 on centos 6.7 (PR job; patty)
  AC_CHECK_HEADERS([libpq-fe.h], [hootFoundLibpqHeaders=yes; break;])
  # pg 9.2 on centos 6.7 (RPM job)
  AC_CHECK_HEADERS([/usr/pgsql-9.2/include/libpq-fe.h], [hootFoundLibpqHeaders=yes; break;])
  AS_IF([test "x$hootFoundLibpqHeaders" != "xyes"], [AC_MSG_ERROR([Unable to find libpq-fe.h])])

])
