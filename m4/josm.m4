AC_DEFUN([JOSM_INIT],[
  AC_SUBST(HAS_JOSM)

  AC_LANG_PUSH([C++])

  # Doing the JNI checks as part of the JOSM checks, since JOSM integration is the only thing using
  # JNI. If anything else needs to use JNI in the future, then the JNI checks may need to be broken
  # out of here.

  AS_IF([test -z "$JAVA_HOME"], [AC_MSG_FAILURE([JAVA_HOME is not set. Source SetupEnv.sh or install a JDK.])])

  JVM_LIB_DIR=""
  for hootJniLibDir in "$JAVA_HOME/lib/server" "$JAVA_HOME/jre/lib/amd64/server"; do
    AS_IF([test -d "$hootJniLibDir"], [JVM_LIB_DIR="$hootJniLibDir"; break])
  done
  AS_IF([test -z "$JVM_LIB_DIR"], [AC_MSG_FAILURE([Unable to find libjvm under JAVA_HOME])])

  CPPFLAGS="-I$JAVA_HOME/include -I$JAVA_HOME/include/linux ${CPPFLAGS}"
  LIBS="-L$JVM_LIB_DIR -ljvm ${LIBS}"

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
