# lcov provides code coverage in testing.
AC_DEFUN([COVERAGE_INIT],[
  AC_SUBST(HAS_COVERAGE) 

  AC_ARG_WITH(coverage,
    [AS_HELP_STRING([--with-coverage],[Include code coverage for debugging (ARG=yes or no)])],
    [],
    [with_coverage=no])

  if test "$with_coverage" = "no" ; then
    COVERAGE_ENABLED=no
    AC_MSG_NOTICE(Code coverage support disabled.)
  else
    if test "$with_coverage" = "yes" -o "$with_coverage" = "" ; then
      AC_CHECK_PROG(LCOV_FOUND, lcov, [yes], [no])
      AS_IF([test "x$LCOV_FOUND" != "xyes"], [AC_MSG_ERROR([Unable to find lcov])])
      AC_SUBST(HAS_COVERAGE, "coverage")
      echo Code coverage support enabled.
    else
      AC_MSG_NOTICE(Code coverage support disabled.)
    fi
  fi

])
