# hoot-services provides web services for hootenanny
AC_DEFUN([SERVICES_INIT],[
  AC_SUBST(HAS_SERVICES) 

  AC_ARG_WITH(services,
    [AS_HELP_STRING([--with-services],[Build hoot-services (ARG=yes or no)])],
    [])

  # Default to the services being disabled.
  if test "$with_services" = "no" -o "$with_services" = ""; then
    SERVICES_ENABLED=no
    AC_MSG_NOTICE(Hootenanny Web Services disabled.)
  else
    AC_CHECK_PROG(JAVA_FOUND, java, [yes], [no])
    AC_CHECK_PROG(MVN_FOUND, mvn, [yes], [no])
    AC_DEFINE([HAVE_SERVICES], [1], [Web services are present])
    if test "$with_services" = "yes" ; then
      AS_IF([test "x$JAVA_FOUND" != "xyes"], [AC_MSG_ERROR([Unable to find java])])
      AS_IF([test "x$MVN_FOUND" != "xyes"], [AC_MSG_ERROR([Unable to find mvn])])
      AC_SUBST(HAS_SERVICES, "services")
      echo Hootenanny Web Services enabled.
    else
      AC_MSG_NOTICE(Hootenanny Web Services disabled.)
    fi
  fi

])
