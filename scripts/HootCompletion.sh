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
    # sed commands
    # /^$/d - deletes empty lines
    # /^\S/d - deletes lines that begin with a non-whitespace character
    # s/^\s+(\S+)\s+.+$/\1/ - removes everything from the help command except the actual command
    OPTIONS=`hoot --help | sed -E '/^$/d;/^\S/d;s/^\s+(\S+)\s+.+$/\1/'`
    COMPREPLY=( $( compgen -W '$OPTIONS' -- $cur ) )
  fi

  if [ $COMP_CWORD -gt 1 ]; then
    case "$cur" in
        -*)
            OPTIONS='--define -D --conf -C --trace --debug --info --status --warn --error --fatal'
            COMPREPLY=( $(compgen -W '$OPTIONS' -- $cur ) )
            ;;
        *)
            COMPREPLY=( $(compgen -f -- ${cur}) )
            ;;
    esac

    case "$prev" in
         -C|--conf)
             # OPTIONS will contain all algorithm and conflation config files, including the Testing.conf file
             OPTIONS=`pushd $HOOT_HOME/conf/core/ > /dev/null; ls *Algorithm.conf; popd > /dev/null`
             OPTIONS+=' Testing.conf '
             OPTIONS+=`pushd $HOOT_HOME/conf/core/ > /dev/null; ls *Conflation.conf; popd > /dev/null`
             COMPREPLY=( $(compgen -W '$OPTIONS' -- $cur ) )
             # Include all directories
             COMPREPLY+=( $(compgen -d -- $cur ) )
             # Include all .conf files in the $cur directory
             COMPREPLY+=( $(compgen -f -X '!*.conf' -- $cur ) )
             ;;
         -D|--define)
             # Use hoot info to list all of the options from ConfigOptions.asciidoc
             OPTIONS=`hoot info --config-options`
             COMPREPLY=( $(compgen -W '$OPTIONS' -- $cur ) )
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

  OPTIONS='--current --quick --slow --glacial --all --quick-only --slow-only --glacial-only --case-only --single --names --all-names --fatal --error --status --warn --info --verbose --debug --trace --diff --include --exclude --parallel --color'
  case "$cur" in
    *)
        COMPREPLY=( $( compgen -W '$OPTIONS' -- $cur ) )
        ;;
  esac

  case "$prev" in
    --single)
        TESTS=`HootTest --all-names`
        COMPREPLY=( $( compgen -W '$TESTS' -- $cur ) )
        ;;
  esac

  return 0
}

complete -F _HootTest -o filenames HootTest
