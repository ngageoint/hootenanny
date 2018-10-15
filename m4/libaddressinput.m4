# https://github.com/googlei18n/libaddressinput
AC_DEFUN([LIBADDRESSINPUT_INIT],[
  AC_SUBST(HAS_LIBADDRESSINPUT) 

  AC_LANG_PUSH([C++])

  CPPFLAGS="-I/usr/include/libaddressinput ${CPPFLAGS}"
  LIBS="-laddressinput -L/usr/lib64 ${LIBS}"

  AC_CHECK_HEADERS(
    [libaddressinput/address_formatter.h], 
    [hootFoundLibaddressinputHeaders=yes; break;], 
    [AC_MSG_FAILURE("Unable to find libaddressinput address_formatter header"); break;])
  AC_CHECK_HEADERS(
    [libaddressinput/address_normalizer.h], 
    [hootFoundLibaddressinputHeaders=yes; break;], 
    [AC_MSG_FAILURE("Unable to find libaddressinput address_normalizer header"); break;])
  AC_CHECK_HEADERS(
    [libaddressinput/address_validator.h], 
    [hootFoundLibaddressinputHeaders=yes; break;], 
    [AC_MSG_FAILURE("Unable to find libaddressinput address_validator header"); break;])

  AC_LINK_IFELSE(
    [AC_LANG_PROGRAM(
      [[#include <libaddressinput/address_data.h> 
        #include <libaddressinput/address_formatter.h>]],
      [[i18n::addressinput::AddressData address; 
        std::string result; 
        i18n::addressinput::GetStreetAddressLinesAsSingleLine(address, &result);]])],
    [],
    [hootFoundLibaddressinputHeaders=no; AC_MSG_FAILURE("Unable to link to libaddressinput address_formatter"); break;])
  AC_LINK_IFELSE(
    [AC_LANG_PROGRAM(
      [[#include <libaddressinput/address_normalizer.h>]],
      [[i18n::addressinput::AddressNormalizer normalizer(NULL);]])],
    [],
    [hootFoundLibaddressinputHeaders=no; AC_MSG_FAILURE("Unable to link to libaddressinput address_normalizer"); break;])
  AC_LINK_IFELSE(
    [AC_LANG_PROGRAM(
      [[#include <libaddressinput/address_validator.h>]],
      [[i18n::addressinput::AddressValidator validator(NULL);]])],
    [],
    [hootFoundLibaddressinputHeaders=no; AC_MSG_FAILURE("Unable to link to libaddressinput address_validator"); break;])

  AC_LANG_POP
])

