# https://github.com/googlei18n/libphonenumber
AC_DEFUN([LIBPHONENUMBER_INIT],[
  AC_SUBST(HAS_LIBPHONENUMBER) 

  CPPFLAGS="-I/usr/include/phonenumbers ${CPPFLAGS}"
  LIBS="-lphonenumber -L/usr/lib64 ${LIBS}"

  #AC_CHECK_LIB([libphonenumber], [i18n::phonenumbers::PhoneNumberUtil::GetInstance], [], AC_MSG_FAILURE("libphonenumber is required."))

  # TODO: this isn't actually doing anything (?)
  AC_LINK_IFELSE(
				[AC_LANG_PROGRAM(
					[[#include <phonenumbers/phonenumberutil.h>]],
					[[i18n::phonenumbers::PhoneNumberUtil::GetInstance();]])],
				[hootFoundLibphonenumberHeaders=yes; break;])

  #AC_CHECK_HEADERS([libphonenumber/phonenumber.h], [hootFoundLibphonenumberHeaders=yes; break;])
  #AS_IF([test "x$hootFoundLibphonenumberHeaders" != "xyes"], [AC_MSG_ERROR([Unable to find libphonenumber])])

])

