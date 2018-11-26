# https://github.com/googlei18n/libphonenumber
AC_DEFUN([LIBPHONENUMBER_INIT],[
  AC_SUBST(HAS_LIBPHONENUMBER) 

  AC_LANG_PUSH([C++])

  CPPFLAGS="-I/usr/include/phonenumbers -I/usr/include/phonenumbers/geocoding ${CPPFLAGS}"
  LIBS="-lphonenumber -lgeocoding -L/usr/lib64 ${LIBS}"

  AC_CHECK_HEADERS(
    [phonenumbers/phonenumberutil.h], 
    [hootFoundLibphonenumberHeaders=yes; break;], 
    [AC_MSG_FAILURE("Unable to find libphonenumber header"); break;])
  AC_LINK_IFELSE(
    [AC_LANG_PROGRAM(
      [[#include <phonenumbers/phonenumberutil.h>]],
      [[i18n::phonenumbers::PhoneNumberUtil::GetInstance();]])],
    [],
    [hootFoundLibphonenumberHeaders=no; AC_MSG_FAILURE("Unable to link to libphonenumber"); break;])

  AC_CHECK_HEADERS(
    [phonenumbers/geocoding/phonenumber_offline_geocoder.h], 
    [hootFoundLibphonenumberGeocodingHeaders=yes; break;], 
    [AC_MSG_FAILURE("Unable to find libphonenumber geocoding header"); break;])
  AC_LINK_IFELSE(
    [AC_LANG_PROGRAM(
      [[#include <phonenumbers/geocoding/phonenumber_offline_geocoder.h>]],
      [[i18n::phonenumbers::PhoneNumberOfflineGeocoder geocoder;]])],
    [],
    [hootFoundLibphonenumberGeocodingHeaders=no; AC_MSG_FAILURE("Unable to link to libphonenumber geocoding"); break;])

  AC_LANG_POP
])

