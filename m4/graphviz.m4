AC_DEFUN([GRAPHVIZ_INIT],[
  AC_SUBST(HAS_GRAPHVIZ) 

  AC_LANG_PUSH([C++])

  DOT_PATH="`which dot > /dev/null`"
  if test $? -eq 0; then
    AC_DEFINE([HAS_GRAPHVIZ], [1], [Include Graphviz])
    AC_SUBST(HAS_GRAPHVIZ, "graphviz")
    echo "Graphviz functions enabled."
  else
    AC_MSG_FAILURE("Graphviz dot program not found"); break;
  fi
  
  AC_LANG_POP
])

