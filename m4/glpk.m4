# GLPK routine
# http://www.gnu.org/software/glpk/
AC_DEFUN([GLPK_INIT],[
  AC_SUBST(HAS_GLPK) 

  AC_CHECK_LIB([glpk], [main], [], AC_MSG_FAILURE("GLPK is required."))

  AC_CHECK_HEADERS([glpk.h glpk/glpk.h], [hootFoundGlpkHeaders=yes; break;])
  AS_IF([test "x$hootFoundGlpkHeaders" != "xyes"], [AC_MSG_ERROR([Unable to find glpk.h])])

])

