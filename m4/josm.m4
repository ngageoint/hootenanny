AC_DEFUN([JOSM_INIT],[
  AC_SUBST(HAS_JOSM) 

  AC_ARG_WITH(rnd,
    [AS_HELP_STRING([--with-josm],[Include support for validation/cleaning with JOSM (ARG=yes or no)])],
    [])

  # Default to JOSM enabled.
  if test "$with_josm" = "no" -o "$with_josm" = ""; then
    JOSM_ENABLED=no
    AC_MSG_NOTICE("JOSM functions disabled.")
  else
    AC_DEFINE([HAVE_JOSM], [1], [Include JOSM])
    AC_SUBST(HAS_JOSM, "josm")
    echo "JOSM functions enabled."
  fi

])
