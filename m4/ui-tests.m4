# Allows for running the hoot-ui cucumber tests
AC_DEFUN([UI_TESTS_INIT],[
  AC_SUBST(HAS_UI_TESTS) 

  AC_ARG_WITH(uitests,
    [AS_HELP_STRING([--with-uitests],[Build hoot-ui tests (ARG=yes or no)])],
    [])

  # Default to disabled.
  if test "$with_uitests" = "no" -o "$with_uitests" = ""; then
    UI_TESTS_ENABLED=no
    AC_MSG_NOTICE(Hootenanny UI tests disabled.)
  else
    AC_DEFINE([HAVE_UI_TESTS], [1], [UI tests are present])
    if test "$with_uitests" = "yes" ; then

      if test "$TOMCAT8_HOME" == ""; then
        AC_MSG_ERROR([Tomcat not installed.  Set TOMCAT8_HOME environment variable to installed location.])
      fi

      # ui tests deploy the hoot ui to tomcat, which installs the mapnik server that needs npm
      AC_CHECK_PROG(NPM_FOUND, npm, [yes], [no])
      AS_IF([test "x$NPM_FOUND" != "xyes"], [AC_MSG_ERROR([Unable to find npm.])])

      # Check for at least Ruby 2.7.x (which includes Gem 3.1.6 and and Bundler 2.1.4).
      CHECK_RUBY(2.7.0)
      CHECK_GEM(3.1.6)
      CHECK_RUBY_GEM(bundler, 2.1.4)

      AC_CHECK_PROG(CHROME_FOUND, google-chrome-stable, [yes], [no])
      AS_IF([test "x$CHROME_FOUND" != "xyes"], [AC_MSG_ERROR([Unable to find chrome.])])
      
      AC_CHECK_PROG(CHROMEDRIVER_FOUND, chromedriver, [yes], [no])
      AS_IF([test "x$CHROMEDRIVER_FOUND" != "xyes"], [AC_MSG_ERROR([Unable to find chromedriver.])])
      
      AC_SUBST(HAS_UI_TESTS, "uitests")
      echo Hootenanny UI tests enabled.

    else
      AC_MSG_NOTICE(Hootenanny UI tests disabled.)
    fi
  fi

])
