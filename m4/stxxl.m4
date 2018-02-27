AC_DEFUN([STXXL_INIT],[
  AC_SUBST(HAS_STXXL) 

  AC_LANG_PUSH([C++])
    AC_CHECK_LIB([stxxl], [main], AC_SUBST(HAS_STXXL, "stxxl"), [AC_MSG_ERROR([STXXL library was not found])])
    AC_CHECK_LIB([stxxl], [main], AC_DEFINE([HAVE_STXXL], [1], [STXXL is present]))
    AC_CHECK_LIB([stxxl], [main])
  AC_LANG_POP

])
