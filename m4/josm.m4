AC_DEFUN([JOSM_INIT],[
  AC_SUBST(HAS_JOSM)

  AC_LANG_PUSH([C++])

  # Doing the JNI checks as part of the JOSM checks, since JOSM integration is the only thing using
  # JNI. If anything else needs to use JNI in the future, then the JNI checks may need to be broken
  # out of here.

  CPPFLAGS="-I$JAVA_HOME/include -I$JAVA_HOME/include/linux ${CPPFLAGS}"
  LIBS="-L$JAVA_HOME/jre/lib/amd64/server -ljvm ${LIBS}"

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

  AC_DEFINE([HAS_JOSM], [1], [Include JOSM])
  AC_SUBST(HAS_JOSM, "josm")
  echo "JOSM functions enabled."

])
