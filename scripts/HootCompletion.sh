# file: UseGetOpt-2
# hoot parameter-completion

_hoot ()   #  By convention, the function name
{                 #+ starts with an underscore.
  local cur
  # Pointer to current completion word.
  # By convention, it's named "cur" but this isn't strictly necessary.

  COMPREPLY=()   # Array variable storing the possible completions.
  cmd=${COMP_WORDS[1]}
  cur=${COMP_WORDS[COMP_CWORD]}
  prev=${COMP_WORDS[COMP_CWORD-1]}

  if [ $COMP_CWORD -eq 1 ]; then
    OPTIONS=`hoot --help | sed '1,/For detailed/d'`
    COMPREPLY=( $( compgen -W '$OPTIONS' -- $cur ) )
#   Generate the completion matches and load them into $COMPREPLY array.
#   xx) May add more cases here.
#   yy)
#   zz)
  fi

  if [ $COMP_CWORD -gt 1 ]; then
    case "$cur" in
        -*)
            OPTIONS='--debug --define -D --conf -C --info --warn --error'
            COMPREPLY=( $(compgen -W '$OPTIONS' -- $cur ) )
            ;;
        *)
            COMPREPLY=( $(compgen -f -- ${cur}) )
            ;;
    esac
  fi

  return 0
}

complete -F _hoot -o filenames hoot

_HootTest()
{
  local cur
  # Pointer to current completion word.
  # By convention, it's named "cur" but this isn't strictly necessary.

  COMPREPLY=()   # Array variable storing the possible completions.
  cur=${COMP_WORDS[COMP_CWORD]}
  prev=${COMP_WORDS[COMP_CWORD-1]}

  OPTIONS='--current --quick --slow --glacial --all --quiet --core --single --names --all-names --warn --info --debug'
  case "$cur" in
    *)
        COMPREPLY=( $( compgen -W '$OPTIONS' -- $cur ) )
        ;;
  esac

  case "$prev" in
    --single)
    TESTS=`HootTest --all-names`
    COMPREPLY=( $( compgen -W '$TESTS' -- $cur ) );;
  esac

  return 0
}

complete -F _HootTest -o filenames HootTest
