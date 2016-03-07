AC_DEFUN([CHECK_RUBY],
[
  if test "x$RUBY" == "x"; then
    AC_PATH_PROG([RUBY], ruby)
  fi

  if test "x$RUBY" == "x" ; then
    AC_MSG_ERROR([ruby is required])
  fi

  minRubyVersion=[$1]
  AC_MSG_CHECKING([for ruby minimum version $minRubyVersion])

  ## warning about line below: use $ 2 instead of $2 otherwise m4 trys to
  ## interpret, luckily awk doesn't care
  rubyVersion=`$RUBY --version | awk '{print $ 2}'`

  success=no
  AX_COMPARE_VERSION([$rubyVersion],[ge],[$minRubyVersion],
    [AC_MSG_RESULT([$rubyVersion is ok])],
    [AC_MSG_ERROR([ruby version must be >= $minRubyVersion - found $rubyVersion])])
])

AC_DEFUN([CHECK_GEM],
[
  if test "x$GEM" == "x"; then
    AC_PATH_PROG([GEM], gem)
  fi

  minGemVersion=[$1]
  if test "x$GEM" == "x" ; then
    AC_MSG_ERROR([ruby gem not found])
  fi

  AC_MSG_CHECKING([for gem minimum version $minGemVersion])

  gemVersion=`$GEM --version`

  AX_COMPARE_VERSION([$gemVersion],[ge],[$minGemVersion],
    [AC_MSG_RESULT($gemVersion is ok)],
    [AC_MSG_ERROR([gem version must be >= $minGemVersion - found $gemVersion])])
])

AC_DEFUN([CHECK_RUBY_GEM],
[
  rubyGem=[$1]
  minRubyGemVersion=[$2]
  acl_cv_gem=`$GEM list --local | grep "$1 (" | awk '{print $ 2}'| sed 's/(//g' | sed 's/)//g' | sed 's/,//g'`
  rubyGemVersion=$acl_cv_gem
  AC_MSG_CHECKING([for [$1] minimum version $minRubyGemVersion])
  if test x"$acl_cv_gem" == x; then
    AC_MSG_ERROR([missing])
    AC_MSG_NOTICE([type 'gem install $rubyGem' to install])
  else
    AX_COMPARE_VERSION([$rubyGemVersion],[ge],$[minRubyGemVersion], [AC_MSG_RESULT($rubyGemVersion is ok)],
      [AC_MSG_ERROR([$rubyGem version must be >= $minRubyGemVersion - found $rubyGemVersion])])
  fi
])
