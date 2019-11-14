AC_DEFUN([JNI_INIT],[
  AC_SUBST(HAS_JNI) 

  AC_LANG_PUSH([C++])

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
])

