# hoot-ui provides a user interface for hootenanny
AC_DEFUN([UI_INIT],[
  AC_SUBST(HAS_UI) 

  AC_ARG_WITH(ui,
    [AS_HELP_STRING([--with-ui],[Build hoot-ui (ARG=yes or no)])],
    [])

  # Default to the services being disabled.
  if test "$with_ui" = "no" -o "$with_ui" = ""; then
    UI_ENABLED=no
    AC_MSG_NOTICE(Hootenanny UI disabled.)
  else
    AC_CHECK_PROG(NPM_FOUND, npm, [yes], [no])
    AC_DEFINE([HAVE_UI], [1], [UI is present])
    if test "$with_ui" = "yes" ; then
      AS_IF([test "x$NPM_FOUND" != "xyes"], [AC_MSG_ERROR([Unable to find npm])])
      AC_SUBST(HAS_UI, "ui")
      echo Hootenanny UI enabled.
    else
      AC_MSG_NOTICE(Hootenanny UI disabled.)
    fi
  fi

])
