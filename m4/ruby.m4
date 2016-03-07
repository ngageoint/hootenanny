# ==================== Ruby ====================
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
    [
      AC_MSG_RESULT([$rubyVersion is ok])
      ifelse([$2], , :, [$2])
      success=yes
    ],
    [
      AC_MSG_RESULT([ruby version must be >= $minRubyVersion - found $rubyVersion])
      ifelse([$3], , :, [$3])
    ]
  )

  if test "$success" = "yes"; then
    RUBY_TOPDIR=`$RUBY -rrbconfig -e  "print \"#{RbConfig::CONFIG['topdir']}\n\""`
    RUBY_LIBRUBYARG=`$RUBY -rrbconfig -e "print \"#{RbConfig::CONFIG['LIBRUBYARG']}\n\""`
    RUBY_LIBDIR=`$RUBY -rrbconfig -e "print \"#{RbConfig::CONFIG['libdir']}\n\""`
    AC_SUBST(RUBY_TOPDIR)
    AC_SUBST(RUBY_LIBRUBYARG)
    AC_SUBST(RUBY_LIBDIR)
  fi
])

# ==================== Ruby Gem ====================
# Like Perl's CPAN
AC_DEFUN([CHECK_GEM],
[
  if test "x$GEM" == "x"; then
    AC_PATH_PROG([GEM], gem)
  fi

  minGemVersion=[$1]
  success=yes
  if test "x$GEM" == "x" ; then
    AC_MSG_RESULT([ruby gem not found])
    success=no
  fi

  if test "$success" = "yes"; then
    AC_MSG_CHECKING([for gem minimum version $minGemVersion])

    gemVersion=`$GEM --version`

    AX_COMPARE_VERSION([$gemVersion],[ge],[$minGemVersion],
         [AC_MSG_RESULT($gemVersion is ok)],
         [
           AC_MSG_RESULT([gem version must be >= $minGemVersion - found $gemVersion])
           success=no
         ])
  fi;

  if test "$success" = "yes"; then
    ifelse([$2], , :, [$2])
  else
    ifelse([$3], , :, [$3])
  fi
])

##
##  pass module path (e.g. wsdl/soap/wsl2ruby)
##
AC_DEFUN([CHECK_RUBY_GEM],
[
  rubyGem=[$1]
	minRubyGemVersion=[$2]

	AC_CACHE_CHECK([for ruby gem $rubyGem],[$rubyGem],[
          # The original string capture, commented out below, wouldn't work with my version of ruby.  Its possible my
          # changes make this more brittle than it needs to be...will do some testing.
          #acl_cv_gem_[$3]=`$GEM list --local [$1] | egrep "^[$1]" | awk '{print $ 2}'| sed -e 's/@<:@(),@:>@//g'`
          # I know there's a way to do this with one sed expression...
	  acl_cv_gem_[$3]=`$GEM list --local | grep "$1 (" | awk '{print $ 2}'| sed 's/(//g' | sed 's/)//g' | sed 's/,//g'`
	])
	rubyGemVersion=$acl_cv_gem_[$3]
	AC_MSG_CHECKING([for [$1] minimum version $minRubyGemVersion])
  success=yes
	if test x"$acl_cv_gem_[$3]" == x; then
    AC_MSG_RESULT([missing])
    AC_MSG_NOTICE([type 'gem install $rubyGem' to install])
    success=no
  else
	  AX_COMPARE_VERSION([$rubyGemVersion],[ge],$[minRubyGemVersion],
		  [AC_MSG_RESULT($rubyGemVersion is ok)],
		  [
        AC_MSG_RESULT([$rubyGem version must be >= $minRubyGemVersion - found $rubyGemVersion])
        success=no
      ])
  fi
  if test "$success" = "yes"; then
    ifelse([$4], , :, [$4])
  else
    ifelse([$5], , :, [$5])
  fi
])
