# Allows for running the hoot-ui cucumber tests
AC_DEFUN([UI_TESTS_INIT],[
  AC_SUBST(HAS_UI_TESTS) 

  AC_ARG_WITH(ui-tests,
    [AS_HELP_STRING([--with-ui-tests],[Build hoot-ui tests (ARG=yes or no)])],
    [])

  # Default to disabled.
  if test "$with_ui_tests" = "no" -o "$with_ui_tests" = ""; then
    UI_TESTS_ENABLED=no
    AC_MSG_NOTICE(Hootenanny UI tests disabled.)
  else
    AC_DEFINE([HAVE_UI_TESTS], [1], [UI tests are present])
    if test "$with_ui_tests" = "yes" ; then

      if test "$TOMCAT6_HOME" == ""; then
        AC_MSG_ERROR([Tomcat not installed.])
      fi

      # ui tests deploy the hoot ui to tomcat, which installs the mapnik server that needs npm
      AC_CHECK_PROG(NPM_FOUND, npm, [yes], [no])
      AS_IF([test "x$NPM_FOUND" != "xyes"], [AC_MSG_ERROR([Unable to find npm.])])

      have_ruby=yes
      CHECK_RUBY(1.9,[],[have_ruby=no])
      #do we we need to check the gem version?
      #CHECK_GEM(1.3,[],[have_ruby=no])
      CHECK_RUBY_GEM(selenium-cucumber, 2.1.4, selenium-cucumber,[],[have_ruby=no])
      CHECK_RUBY_GEM(capybara, 2.5.0, capybara,[],[have_ruby=no]) #should this be 2.6.2 instead?
      CHECK_RUBY_GEM(capybara-webkit, 1.8.0, capybara-webkit,[],[have_ruby=no])
      CHECK_RUBY_GEM(rspec, 3.4.0, rspec,[],[have_ruby=no])

      if test "$have_ruby" != "no"; then
        AC_MSG_ERROR([Unable to find Cucumber related gems.])
      fi
      
      AC_SUBST(HAS_UI_TESTS, "ui_tests")
      echo Hootenanny UI tests enabled.

    else
      AC_MSG_NOTICE(Hootenanny UI tests disabled.)
    fi
  fi

])
