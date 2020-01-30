AC_DEFUN([JOSM_INIT],[
  AC_SUBST(HAS_JOSM) 

  AC_ARG_WITH(josm,
    [AS_HELP_STRING([--with-josm],[Include support for validation/cleaning with JOSM (ARG=yes or no)])],
    [])

  # Default to JOSM enabled. Not actually checking for any JOSM files here. JOSM jar will be checked for when the JNI Java env is initiated.
  if test "$with_josm" = "no" -o "$with_josm" = ""; then
    JOSM_ENABLED=no
    AC_MSG_NOTICE("JOSM functions disabled.")
  else

    AC_LANG_PUSH([C++])

    # Doing the JNI checks as part of the JOSM checks, since JOSM integration is the only thing using JNI. If anything else needs to use JNI
    # in the future, then the JNI checks may need to be broken out of here.

    CPPFLAGS="-I/usr/lib/jvm/java-1.8.0-openjdk/include -I/usr/lib/jvm/java-1.8.0-openjdk/include/linux ${CPPFLAGS}"
    LIBS="-L/usr/lib/jvm/java-1.8.0-openjdk/jre/lib/amd64/server -ljvm ${LIBS}"
  
    AC_CHECK_HEADERS(
      [jni.h], 
      [hootFoundJniHeaders=yes; break;], 
      [AC_MSG_FAILURE("Unable to find JNI header"); break;])

    AC_LINK_IFELSE(
      [AC_LANG_PROGRAM(
        [[#include <jni.h>]],
        [[JavaVM* vm = NULL;]])],
      [],
      [hootFoundJniHeaders=no; AC_MSG_FAILURE("Unable to link to libjvm"); break;])

    AC_LANG_POP

    AC_DEFINE([HAVE_JOSM], [1], [Include JOSM])
    AC_SUBST(HAS_JOSM, "josm")
    echo "JOSM functions enabled."

  fi

])
