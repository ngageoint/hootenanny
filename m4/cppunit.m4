AC_DEFUN([CPPUNIT_INIT],[
  AC_SUBST(HAS_CPPUNIT) 

  AC_ARG_WITH(cppunit,[  --with-cppunit[=ARG]      Include CPPUnit support (ARG=yes or no)],,)

  if test "$with_cppunit" = "no" ; then
    CPPUNIT_ENABLED=no
    AC_MSG_NOTICE(CPPUnit support disabled.)
  else
    if test "$with_cppunit" = "yes" -o "$with_cppunit" = "" ; then
      AC_CHECK_LIB([cppunit], [main], AC_SUBST(HAS_CPPUNIT, "cppunit"))
      AC_CHECK_LIB([cppunit], [main])
    fi
  fi

])

