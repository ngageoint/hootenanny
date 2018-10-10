# https://github.com/googlei18n/libphonenumber
AC_DEFUN([LIBPHONENUMBER_INIT],[
  AC_SUBST(HAS_LIBPHONENUMBER) 

  AC_CHECK_LIB([libphonenumber], [main], [], AC_MSG_FAILURE("libphonenumber is required."))

  AC_CHECK_HEADERS([libphonenumber/phonenumbermatch.h libphonenumber/phonenumber.h], [hootFoundGlpkHeaders=yes; break;])
  AS_IF([test "x$hootFoundLibphonenumberHeaders" != "xyes"], [AC_MSG_ERROR([Unable to find libphonenumber includes])])

])

