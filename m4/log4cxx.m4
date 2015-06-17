AC_DEFUN([LOG4CXX_INIT],[
  AC_SUBST(HAS_LOG4CXX) 

  AC_ARG_WITH(log4cxx,[  --with-log4cxx[=ARG]      Include log4cxx support (ARG=yes or no)],,)

  if test "$with_log4cxx" = "no" ; then
    LOG4CXX_ENABLED=no
    echo "log4cxx support disabled."
  else
    if test "$with_log4cxx" = "yes" -o "$with_log4cxx" = "" ; then
      AC_CHECK_LIB([log4cxx], [main], AC_SUBST(HAS_LOG4CXX, "log4cxx"))
      AC_CHECK_LIB([log4cxx], [main])
    fi
  fi

])

