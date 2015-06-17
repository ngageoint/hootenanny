
# SYNOPSIS
#
# AX_RESET_HEADERS_CACHE(headers ...)
#
# DESCRIPTION
#
# This macro invalidates the headers cache variables created by previous AC_CHECK_HEADER/AC_CHECK_HEADERS checks.
#
AC_DEFUN([AX_RESET_HEADERS_CACHE], [
    AS_FOR([AX_var], [ax_var], [$1], [
        dnl You can replace "ac_cv_header_" with any prefix from http://www.gnu.org/software/autoconf/manual/html_node/Cache-Variable-Index.html
        AS_VAR_PUSHDEF([ax_Var], [ac_cv_header_${ax_var}])
        AS_UNSET([ax_Var])
        AS_VAR_POPDEF([ax_Var])
    ])
]) # AX_RESET_HEADERS_CACHE

#
# Look for a header file in an include path. If the header file has already 
# been found, do nothing. If the header file exists in the path then add
# the path to hadoop_include_dirs. When this is done there will be no changes
# to CPPFLAGS.
#
# $1 - Include file to look for.
# $2 - CPPFLAGS to use to look.
#
AC_DEFUN([HADOOP_CHECK_HEADER],
[
    shh_CPPFLAGS=$CPPFLAGS

    CPPFLAGS="$hadoop_include_dirs $CPPFLAGS"

    # Have we already found this header file?
    AC_CHECK_HEADERS([$1], [already_found=yes; break;], [already_found=no; break;])

    if test "$already_found" == "no"; then
        AX_RESET_HEADERS_CACHE([$1])
        CPPFLAGS="$2 $CPPFLAGS"
        AC_MSG_NOTICE(checking for $1 in $2:)
        AC_CHECK_HEADERS([$1], [foundIt=yes; break;], [foundIt=no; break;])
        if test "$foundIt" == "yes"; then
            hadoop_include_dirs="$2 $hadoop_include_dirs"
        fi
    fi

    CPPFLAGS=$shh_CPPFLAGS
])

#
# Look for a library with specified link flags. If the library has already been found
# and is in the cache, do nothing. If the library is found then add it to the LIBS
# var. 
# $1 - library to look for
# $2 - optional linker flags to evaluate
# $3 - linker flags to check
#
AC_DEFUN([HADOOP_CHECK_LIB],
[
    shc_LIBS=$LIBS

    # Check to see if user provided parameters fulfill this requirement.
    LIBS="$hadoop_lib_dirs $hadoop_link_flags $LIBS"
    AC_CHECK_LIB([$1], [main], [alreadyFound=yes; break;], [alreadyFound=no; break;], [$2])
    LIBS=$shc_LIBS

    if test "$alreadyFound" == "no"; then
        AC_MSG_NOTICE(checking for $1 in $3:)
        # Reset library cache.
        AS_VAR_PUSHDEF([ax_Var], [ac_cv_lib_$1_main])
        AS_UNSET([ax_Var])
        AS_VAR_POPDEF([ax_Var])
        LIBS="$3 $LIBS"
        AC_CHECK_LIB([$1], [main], [foundIt=yes; break;], [foundIt=no; break;], [$2])
        if test "$foundIt" == "no"; then
            LIBS=$shc_LIBS
        fi
    else
        LIBS=$shc_LIBS
    fi

])

AC_DEFUN([AX_LIB_HADOOP],
[
    AC_ARG_WITH([hadoop-inc],
        AC_HELP_STRING([--with-hadoop-inc=@<:@DIRS@:>@],
            [path(s) to Hadoop headers separated by a single space and prepended with "-I"; defaults to Apache Hadoop (tested with 0.20.2)]
        ),
        [hadoop_include_dirs="$withval"],
        [hadoop_include_dirs=""]
    )

    AC_ARG_WITH([hadoop-lib],
        AC_HELP_STRING([--with-hadoop-lib=@<:@DIRS@:>@],
            [Link options for linking with Hadoop (e.g. -lhdfs -L$HADOOP_HOME/lib/...)]
        ),
        [hadoop_lib="$withval"],
        [hadoop_lib=""]
    )

    AC_LANG_PUSH([C++])

    saved_LIBS="$LIBS"
    saved_CPPFLAGS="$CPPFLAGS"
    
    #adding these link options was necessary to get the CDH build to work; lhadooputils is added
    #in the pp-bin.pro b/c adding it here resulted in a linking error
    LIBS="$hadoop_lib $LIBS -lhdfs -lcrypto"  

    # if the user _hasn't_ specified specific include dirs
    if test "$hadoop_include_dirs" == ""; then
        # Looks for header files in specific paths, the first one found is kept.
        HADOOP_CHECK_HEADER([jni.h], [-I$JAVA_HOME/include -I$JAVA_HOME/include/linux])
        HADOOP_CHECK_HEADER([hadoop/Pipes.hh], [-I$HADOOP_HOME/c++/Linux-amd64-64/include/])
        HADOOP_CHECK_HEADER([hadoop/Pipes.hh], [-I/usr/src/hadoop-0.20/c++/])
        HADOOP_CHECK_HEADER([hdfs.h], [-I$HADOOP_HOME/src/c++/libhdfs/])
        HADOOP_CHECK_HEADER([hdfs.h], [-I/usr/src/hadoop-0.20/c++/libhdfs])
        HADOOP_CHECK_HEADER([pipes/impl/HadoopPipes.cc], [-I$HADOOP_HOME/src/c++/])
        HADOOP_CHECK_HEADER([pipes/impl/HadoopPipes.cc], [-I/usr/src/hadoop-0.20/c++/])
    fi

    # Add any include dirs that the auto-search filled in
    CPPFLAGS="$hadoop_include_dirs $CPPFLAGS"

    AC_MSG_NOTICE(HADOOP_HOME: $HADOOP_HOME)
    AC_MSG_NOTICE(hadoop_include_dirs: $hadoop_include_dirs)

    # if the user _hasn't_ specified specific linker options
    if test "$hadoop_lib" == ""; then
        # Looks for libraries in specific paths, the first one found is kept.
        HADOOP_CHECK_LIB([hdfs], [-lcrypto], [-L$HADOOP_HOME/lib/native/Linux-amd64-64])
        HADOOP_CHECK_LIB([hdfs], [-lcrypto], [-L$HADOOP_HOME/c++/Linux-amd64-64/lib])
        HADOOP_CHECK_LIB([hadooppipes], [-lcrypto], [-L$HADOOP_HOME/lib/native/Linux-amd64-64])
        HADOOP_CHECK_LIB([hadooppipes], [-lcrypto], [-L$HADOOP_HOME/c++/Linux-amd64-64/lib])
        #assuming OpenJDK installation here; this assumption should probably not exist, and this logic
        #moved to its own m4
        HADOOP_CHECK_LIB([jvm], [], [-L${JAVA_HOME}/jre/lib/amd64/server/])
    else
        AC_MSG_NOTICE(hadoop_lib: $hadoop_lib)
    fi

    # Do a final sanity check
    AC_CHECK_HEADERS([hdfs.h], [foundHadoopHeaders=yes; break;], [foundHadoopHeaders=no; break;])
    AC_CHECK_LIB([hdfs], [main], [foundHadoopLib=yes; break;], [foundHadoopLib=no; break;])

    #how do you do an OR statement?
    if test "$JAVA_HOME" == ""; then
      AC_MSG_NOTICE(Hadoop was not found.)
      echo Disabling Hadoop. JAVA_HOME is not set.
      CPPFLAGS="$saved_CPPFLAGS" 
      LIBS="$saved_LIBS" 
    elif test "$foundHadoopHeaders" == "no"; then
      AC_MSG_NOTICE(Hadoop was not found.)
      CPPFLAGS="$saved_CPPFLAGS" 
      LIBS="$saved_LIBS" 
    elif test "$foundHadoopLib" == "no"; then
      AC_MSG_NOTICE(Hadoop was not found.)
      CPPFLAGS="$saved_CPPFLAGS" 
      LIBS="$saved_LIBS"
    else
      AC_MSG_NOTICE(Hadoop found.)
      AC_DEFINE([HAVE_HADOOP], [1], [Hadoop is present])
      AC_SUBST(HAS_HADOOP, "hadoop")
    fi

    AC_LANG_POP
])
