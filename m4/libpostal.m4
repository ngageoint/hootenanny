# https://github.com/openvenues/libpostal/
AC_DEFUN([LIBPOSTAL_INIT],[
  AC_SUBST(HAS_LIBPOSTAL) 

  AC_LANG_PUSH([C++])

  CPPFLAGS="-I/usr/include/libpostal ${CPPFLAGS}"
  LIBS="-lpostal -L/usr/lib64 ${LIBS}"

  AC_CHECK_HEADERS(
    [libpostal/libpostal.h], 
    [hootFoundLibpostalHeaders=yes; break;], 
    [AC_MSG_FAILURE("Unable to find libpostal header"); break;])

  AC_LINK_IFELSE(
    [AC_LANG_PROGRAM(
      [[#include <libpostal/libpostal.h>]],
      [[libpostal_setup(); libpostal_setup_parser(); libpostal_teardown(); libpostal_teardown_parser();]])],
    [],
    [hootFoundLibpostalHeaders=no; AC_MSG_FAILURE("Unable to link to libpostal"); break;])

  AC_LANG_POP
])

