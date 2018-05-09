AC_DEFUN([RND_INIT],[
  AC_SUBST(HAS_RND) 

  AC_ARG_WITH(rnd,
    [AS_HELP_STRING([--with-rnd],[Include research (experimental) functions (ARG=yes or no)])],
    [])

  # Default to the R&D disabled.
  if test "$with_rnd" = "no" -o "$with_rnd" = ""; then
    RND_ENABLED=no
    AC_MSG_NOTICE("R&D experimental functions disabled.")
  else
    AC_DEFINE([HAVE_RND], [1], [Include R&D])
    AC_SUBST(HAS_RND, "rnd")
    echo "R&D experimental functions enabled."
  fi

])
