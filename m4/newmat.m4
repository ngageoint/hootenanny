AC_DEFUN([NEWMAT_INIT],[
  AC_SUBST(HAS_NEWMAT) 

  AC_ARG_WITH(newmat,[  --with-newmat[=ARG]       Include newmat support (ARG=yes or no)],,)

  if test "$with_newmat" = "no" ; then
    NEWMAT_ENABLED=no
    echo "newmat support disabled."
  else
    if test "$with_newmat" = "yes" -o "$with_newmat" = "" ; then
      AC_CHECK_LIB([newmat], [main], AC_SUBST(HAS_NEWMAT, "newmat"))
      AC_CHECK_LIB([newmat], [main])
    fi
  fi

])

