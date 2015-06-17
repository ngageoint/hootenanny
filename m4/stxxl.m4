AC_DEFUN([STXXL_INIT],[
  AC_SUBST(HAS_STXXL) 

  AC_ARG_WITH(stxxl,[  --with-stxxl[=ARG]        Include stxxl support (ARG=yes or no)],,)

  if test "$with_stxxl" = "no" ; then
    STXXL_ENABLED=no
    echo "stxxl support disabled."
  else
    if test "$with_stxxl" = "yes" -o "$with_stxxl" = "" ; then
      AC_CHECK_LIB([stxxl], [main], AC_SUBST(HAS_STXXL, "stxxl"))
      AC_CHECK_LIB([stxxl], [main], AC_DEFINE([HAVE_STXXL], [1], [STXXL is present]))
      AC_CHECK_LIB([stxxl], [main])
    fi
  fi

])

