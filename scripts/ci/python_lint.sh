#!/bin/bash
# This script allows a user to lint a python file or directory of python files

# global variables
LINT_ALL=""
LINT_SINGLE=""
TARGET_MOUNT="/usr/tmp"
LOG_FILE="lint.log"
DOCKER_CONTAINER=""
POPTIONS=""

# public dockerhub image created by a member of the hoot team
runLintContainer()
{
        echo "Running lint container..."
        docker run -i -d \
               --name hoot_lint \
               zappedbyvoltz/pycodestyle &> /dev/null
        sleep 1
        DOCKER_CONTAINER_ID=`docker ps -f "name=hoot_lint" -q`
        if [ -z "${DOCKER_CONTAINER_ID}" ]; then
            echo "Failed to start Docker container."
            exit 1
        fi
}

# in order to only lint files requested per run, the container has to be re-ran
# because we copy the files into the container and a previous run would have
# old files persist
cleanRunningContainer()
{
    echo "Cleaning container..."
    docker container stop hoot_lint &> /dev/null
    docker container rm hoot_lint &> /dev/null
}

# want to start with a clean container, so that if the user runs this script more than
# once then they're not getting more log data than expected
LintContainerCheck()
{
    CLEAN=$1
    if [[ ! -z $( docker ps -a -f "name=hoot_lint" -q ) && ${CLEAN} == 1 ]]; then
        cleanRunningContainer
    elif [[ ! -z $( docker ps -a -f "name=hoot_lint" -q ) && ${CLEAN} == 0 ]]; then
        cleanRunningContainer
        runLintContainer
    else
        runLintContainer
    fi
}

# lint a directory of python files, expects a relative path
lintAllPythonFiles()
{
    if [ -f ${LOG_FILE} ]; then
        rm -f ${LOG_FILE}
        touch ${LOG_FILE}
    fi
    # copy directory into container
    docker cp ${LINT_ALL} ${DOCKER_CONTAINER_ID}:${TARGET_MOUNT}
    # find all python files copied over to container
    docker exec -i ${DOCKER_CONTAINER_ID} find ${TARGET_MOUNT} -follow | grep -v \\.pyc | grep \\.py > .lintfile
    for _file in $( cat .lintfile )
    do
        echo "Linting file: '${_file}'" | sed "s/$( echo ${TARGET_MOUNT} | sed 's/\//\\\//g' )/$( echo ${LINT_ALL} | sed 's/\//\\\//g' )/g"
        # lint all python files inside container and print stdout to log file for user to review
        docker exec -i ${DOCKER_CONTAINER_ID} pycodestyle ${POPTIONS} ${_file} >> ${LOG_FILE}
    done
    # swap relative path provided by user with path on container. Makes readability easier on user
    sed -i "s/$( echo ${TARGET_MOUNT} | sed 's/\//\\\//g' )/$( echo ${LINT_ALL} | sed 's/\//\\\//g' )/g" ${LOG_FILE}
    # remove tmp file
    rm -f .lintfile
}

# lint a single python file, expects a relative path
lintSinglePythonFile()
{
    # copy file over to container
    docker cp ${LINT_SINGLE} ${DOCKER_CONTAINER_ID}:${TARGET_MOUNT}
    # manipulate strings to swap file paths in log, makes readability easier on user
    FILENAME=`echo ${LINT_SINGLE##*/}`
    RELATIVE_PATH=`echo ${LINT_SINGLE%/*}`
    echo "Linting file: ${LINT_SINGLE}"
    # lint file that the user requested and print stdout to file on host machine
    docker exec -i ${DOCKER_CONTAINER_ID} pycodestyle ${POPTIONS} ${TARGET_MOUNT}/${FILENAME} > ${LOG_FILE}
    # swap path on container to relative path on host, make readability easier on user
    sed -i "s/$( echo ${TARGET_MOUNT} | sed 's/\//\\\//g' )/$( echo ${RELATIVE_PATH} | sed 's/\//\\\//g' )/g" ${LOG_FILE}
}

# usage statement to help user know script's capabilities
showUsage()
{
    RTN_CODE=$1
    echo """Usage: $0 [-a|-f]
-a|--all                  Run PEP8 lint against all python files found recursively
-f|--file                 Run PEP8 lint against a single python file
                            *Requires relative path to file location
-h|--help                 show this usage page
-l|--log-file             Change the storage and filename of the log file
                            *Defaults to ./lint.log
-p|--pycodestyle-options  Pass arguements to pycodestyle inside docker container
                            *Requires to be entered as single string
  Options:
    --version            show program's version number and exit
    -h, --help           show this help message and exit
    -v, --verbose        print status messages, or debug with -vv
    -q, --quiet          report only file names, or nothing with -qq
    -r, --repeat         (obsolete) show all occurrences of the same error
    --first              show first occurrence of each error
    --exclude=patterns   exclude files or directories which match these comma
                         separated patterns (default:
                         .svn,CVS,.bzr,.hg,.git,__pycache__,.tox)
    --filename=patterns  when parsing directories, only check filenames matching
                         these comma separated patterns (default: *.py)
    --select=errors      select errors and warnings (e.g. E,W6)
    --ignore=errors      skip errors and warnings (e.g. E4,W) (default:
                         E121,E123,E126,E226,E24,E704,W503,W504)
    --show-source        show source code for each error
    --show-pep8          show text of PEP 8 for each error (implies --first)
    --statistics         count errors and warnings
    --count              print total number of errors and warnings to standard
                         error and set exit code to 1 if total is not null
    --max-line-length=n  set maximum allowed line length (default: 79)
    --max-doc-length=n   set maximum allowed doc line length and perform these
                         checks (unchecked if not set)
    --hang-closing       hang closing bracket instead of matching indentation of
                         opening bracket's line
    --format=format      set the error format [default|pylint|<custom>]
    --diff               report changes only within line number ranges in the
                         unified diff received on STDIN
"""
    exit "${RTN_CODE}"
}

# help prevent user error from running this script for it's purpose
usageLogic()
{
    if [[  -z ${LINT_ALL} &&  -z ${LINT_SINGLE} ]]; then
        showUsage "1"
    elif [[ ! -z ${LINT_ALL} && ! -z ${LINT_SINGLE} ]]; then
        showUsage "1"
    fi
}

# create an arguement parser to make usage and data output more dynamic
usage()
{
    TEMP=`getopt -o a:f:hl:p: -l all:,file:,help,log-file:,pycodestyle-options: -- "$@"`
    eval set -- "${TEMP}"
    while true
    do
        case "$1" in
            -a|-all)
                LINT_ALL=$2
                shift 2 ;;
            -f|--file)
                LINT_SINGLE=$2
                shift 2 ;;
            -h|--help)
                showUsage "0" ;;
            -l|--log-file)
                LOG_FILE=$2
                shift 2 ;;
            -p|--pycodestyle-options)
                POPTIONS=$2
                shift 2 ;;
            --)
                shift
                break ;;
        esac
    done
    usageLogic
}

#----------- Main -----------#
usage "${@}"
LintContainerCheck "0"
if [ ! -z ${LINT_ALL} ]; then
    lintAllPythonFiles
elif [ ! -z ${LINT_SINGLE} ]; then
    lintSinglePythonFile
fi
echo "Lint output stored in '${LOG_FILE}'"
LintContainerCheck "1"
