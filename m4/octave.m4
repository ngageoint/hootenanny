# Octave routine
# http://www.gnu.org/software/glpk/
AC_DEFUN([OCTAVE_INIT],[
  AC_SUBST(HAS_OCTAVE) 

  AC_ARG_WITH(octave,
    [AS_HELP_STRING([--with-octave],[Include octave support (ARG=yes or no)])],
    [])

  if test "$with_octave" = "no" ; then
    OCTAVE_ENABLED=no
    AC_MSG_NOTICE(octave support disabled.)
  else
    AC_CHECK_LIB([octave], [main], AC_SUBST(HAS_OCTAVE, "octave"))
    if test "$HAS_OCTAVE" = "octave"; then
        AC_CHECK_LIB([octave], [main])
        AC_CHECK_LIB([octinterp], [main])
        AC_CHECK_LIB([cruft], [main])

        CPPFLAGS="$CPPFLAGS -I`octave-config -p OCTINCLUDEDIR`/../"
        AC_CHECK_HEADERS([octave/config.h], [], [AC_MSG_ERROR([Unable to find octave headers])])
    fi
  fi

])

