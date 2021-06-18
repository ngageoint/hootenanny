#L                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.65])
m4_include([m4/version.m4])
AC_INIT([hootenanny], [VERSION_NUMBER], [https://github.com/ngageoint/hootenanny])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([hoot-core/src])

# use C++ 11
AC_LANG([C++])
AX_CXX_COMPILE_STDCXX_11([], [mandatory])

###
# Checks for programs.
###
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_RANLIB
LT_INIT

###
# Check for Libraries
###

# GDAL
m4_include([m4/ax_lib_gdal.m4])
AX_LIB_GDAL([1.4.4])
LIBS="$LIBS $GDAL_RPATH $GDAL_LDFLAGS"
CPPFLAGS="$CPPFLAGS $GDAL_CFLAGS"
if test "${GDAL_OGR_ENABLED}" = "yes" ; then
  ogrinfo --formats > /dev/null
  OGR_FORMAT_EXIT_STATUS=`echo $?`
  if test "$OGR_FORMAT_EXIT_STATUS" != "0"; then
    AC_MSG_ERROR(["There is an error with one or more GDAL file formats."]);
  fi
fi
if test "${GDAL_VERSION}" = "1.9.1" ; then
  AC_MSG_WARN(["GDAL v1.9.1 is known to have a bug in FileGDB export where the file can't be opened by ArcMap 10.2. GDAL v1.10.1 is known to work."]);
fi
if test "${GDAL_VERSION}" != "3.2.2" ; then
  AC_MSG_WARN([Most development occurs with GDAL v3.2.2, using v$GDAL_VERSION. Please run all tests after build (make test).]);
fi

# GEOS
m4_include([m4/geos.m4])
# We haven't tested with anything below 3.2.2
GEOS_INIT(3.2.2)
if test "${HAVE_GEOS}" = "yes" ; then
  AC_MSG_NOTICE([Using C++ API from GEOS $GEOS_VERSION])
  LIBS="${LIBS} ${GEOS_LIBS}"
  CPPFLAGS="${CPPFLAGS} ${GEOS_CFLAGS}"
fi
if test "${GEOS_VERSION}" = "3.3.2" ; then
  AC_MSG_WARN([There is a serious bug in GEOS v3.3.2 distance calculations. GEOS v3.2.2 is known to work.]);
fi
if test "${GEOS_VERSION}" != "3.9.1" ; then
  AC_MSG_WARN([Most development occurs with GEOS v3.9.1, using v$GEOS_VERSION. Please run all tests after build (make test).]);
fi

# Python
m4_include([m4/python.m4])
saved_LIBS=$LIBS
saved_CPPFLAGS=$CPPFLAGS
AC_PYTHON_DEVEL([2.4.0])
LIBS="$saved_LIBS $PYTHON_LDFLAGS"
CPPFLAGS="$saved_CPPFLAGS $PYTHON_CPPFLAGS"

# OpenCV - the library names change from version to version so we have to search for the right name.
AC_LANG_PUSH([C++])
AC_SEARCH_LIBS([cvSmooth], [opencv_imgproc opencv_core opencv_highgui cxcore cv], [], [AC_MSG_ERROR([A necessary OpenCV library was not found])])
AC_SEARCH_LIBS([cvCreateImage], [opencv_imgproc opencv_core opencv_highgui cxcore cv], [], [AC_MSG_ERROR([A necessary OpenCV library was not found])])
AC_CHECK_HEADERS([opencv2/opencv.hpp opencv/cv.h], [hootFoundCvHeaders=yes; break;])
AS_IF([test "x$hootFoundCvHeaders" != "xyes"], [AC_MSG_ERROR([Unable to find the opencv headers])])

AC_COMPILE_IFELSE(
    [
        AC_LANG_PROGRAM([[
#ifdef HAVE_OPENCV2_OPENCV_HPP
# include <opencv2/opencv.hpp>
#endif

#ifdef HAVE_OPENCV_CV_H
# include <opencv/cv.h>
#endif
using namespace cv;
]], 
        [[
Mat m1, m2, m3; 
calcCovarMatrix(m1, m2, m3, COVAR_NORMAL | COVAR_ROWS | COVAR_SCALE, CV_64F);
]])
    ],
    [],
    AC_MSG_FAILURE("OpenCV version is incompatible. Version 2.4.1 is known to work."))

AC_LANG_POP

# Boost
m4_include([m4/boost.m4])
AX_LIB_BOOST()

# Qt
AC_CHECK_HEADERS([qt5/QtCore/QtCoreVersion])

# Protocol Buffers
AC_SEARCH_LIBS([_ZTIN6google8protobuf20FieldDescriptorProtoE], [protobuf], [], [AC_MSG_ERROR([Protocol buffers library was not found])])

AC_CHECK_LIB([icui18n], [main], [], [echo "Error! icui18n (libicu) is required."; exit -1])
AC_CHECK_LIB([icuio], [main], [], [echo "Error! icuio (libicu) is required."; exit -1])
AC_CHECK_LIB([iculx], [main], [], [echo "Error! iculx (libicu) is required."; exit -1])
AC_CHECK_LIB([icutu], [main], [], [echo "Error! icutu (libicu) is required."; exit -1])
AC_CHECK_LIB([icuuc], [main], [], [echo "Error! icuuc (libicu) is required."; exit -1])
#AC_CHECK_LIB([pthread], [main], [], [echo "Error! pthread is required."; exit -1])

# libpostal
m4_include([m4/libpostal.m4])
LIBPOSTAL_INIT()

# libphonenumber
m4_include([m4/libphonenumber.m4])
LIBPHONENUMBER_INIT()

# liboauthcpp
m4_include([m4/liboauthcpp.m4])
LIBOAUTHCPP_INIT()

##
# Optional
##

# Include services
m4_include([m4/services.m4])
SERVICES_INIT()

# Include R&D
# This enables research specific functionality. Things that aren't ready for prime time.
m4_include([m4/rnd.m4])
RND_INIT()

# Include JOSM
# This enables validation/cleaning functionality from JOSM.
m4_include([m4/josm.m4])
JOSM_INIT()

# Include UI tests
m4_include([m4/ax_compare_version.m4])
m4_include([m4/ruby.m4])
m4_include([m4/ui-tests.m4])
UI_TESTS_INIT()

# Code Coverage
m4_include([m4/coverage.m4])
COVERAGE_INIT()

# CPPUnit
m4_include([m4/cppunit.m4])
CPPUNIT_INIT()

# GLPK
m4_include([m4/glpk.m4])
GLPK_INIT()

# libpq - part of Postgres
m4_include([m4/ax_lib_postgresql.m4])
AX_LIB_POSTGRESQL(9.1)
LIBS="${LIBS} ${POSTGRESQL_LDFLAGS} ${POSTGRESQL_LIBS}"
CPPFLAGS="${CPPFLAGS} ${POSTGRESQL_CPPFLAGS}"

# Newmat
m4_include([m4/newmat.m4])
NEWMAT_INIT()

# STXXL
m4_include([m4/stxxl.m4])
STXXL_INIT()

m4_include([m4/precision.m4])
CHECK_PRECISION_INIT()

m4_include([m4/nodejs.m4])
NODEJS_INIT()

# Checks for header files.
AC_CHECK_HEADERS([float.h limits.h malloc.h stdint.h stdlib.h string.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_SIZE_T

# Checks for library functions.
AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([Makefile.inc])
AC_CONFIG_FILES([Configure.pri])
AC_CONFIG_HEADERS([config.h])
AC_OUTPUT
