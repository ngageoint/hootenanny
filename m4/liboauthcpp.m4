AC_DEFUN([LIBOAUTHCPP_INIT],[
  AC_SUBST(HAS_LIBOAUTHCPP) 

  AC_LANG_PUSH([C++])

  CPPFLAGS="-I/usr/liboauthcpp/liboauthcpp.h ${CPPFLAGS}"
  LIBS="-loauthcpp -L/usr/lib64 ${LIBS}"

  AC_CHECK_HEADERS(
    [liboauthcpp/liboauthcpp.h], 
    [hootFoundLiboauthcppHeaders=yes; break;], 
    [AC_MSG_FAILURE("Unable to find liboauthcpp header"); break;])

  AC_LINK_IFELSE(
    [AC_LANG_PROGRAM(
      [[#include <liboauthcpp/liboauthcpp.h>]],
      [[OAuth::Client::initialize();]])],
    [],
    [hootFoundLiboauthcppHeaders=no; AC_MSG_FAILURE("Unable to link to liboauthcpp"); break;])

  AC_LANG_POP
])

