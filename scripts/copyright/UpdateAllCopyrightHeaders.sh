#!/bin/bash
# Var for exit code
# Exit codes 1-5 are reserved for UpdateDirCopyrightHeaders.sh (the child script)
# Exit codes 6-11 are reserved for this script, UpdateAllCopyrightHeaders.sh (the parent script)
# Exit codes:
#    0 = Success.
#    1 = LicenseTemplate.txt not found.
#    2 = Need at least one arg which would be the log filename.
#    3 = Unrecognized .log file name.
#    4 = Unrecognized 2nd argument.
#    5 = Too many arguments.
#    6 = Unrecognized parameter.
#    7 = Too many arguments.
#    8 = There are files that need their copyright headers updated.
#    9 = Unrecognized updateMode.
#   10 = Python error(s) found.
#   11 = Log file not found.
#   12 = Search directory not found

GetExitCodeText(){
    if [ $1 > 0 ]; then
        case "$1" in
        "0")
            echo "    Success!"
            ;;
        "1")
            echo "    Error: LicenseTemplate.txt not found"
            ;;
        "2")
            echo "    Error: Need at least one arg which would be the log filename in call to UpdateDirCopyrightHeaders.sh"
            ;;
        "3")
            echo "    Error: Unrecognized .log file name in UpdateDirCopyrightHeaders.sh"
            ;;
        "4")
            echo "    Error: Unrecognized 2nd argument in call to UpdateDirCopyrightHeaders.sh"
            ;;
        "5")
            echo "    Error: Too many arguments in call to UpdateDirCopyrightHeaders.sh"
            ;;
        "6")
            echo "    Error: Unrecognized parameter"
            ;;
        "7")
            echo "    Error: Too many arguments"
            ;;
        "8")
            echo "    Warning: There are files that need their copyright headers updated. (Please run UpdateAllCopyrightHeaders.sh --update)"
            ;;
        "9")
            echo "    Error: Unrecognized updateMode"
            ;;
        "10")
            echo "    Error: Python error(s) found"
            ;;
        "11")
            echo "    Error: Log file not found"
            ;;
        "12")
            echo "    Error: Search directory not found"
            ;;
        *)
            echo "    Error: Unknown"
            ;;
        esac
    fi
}


ExitScript() {
    echo "Exit code: $1"
    GetExitCodeText $1
    exit $1
}

updateMode=false
exitCode=0

# Determine the log file name.
NOW=$(date +"%Y-%m-%d_%H:%M:%S")
logBase="$HOOT_HOME/updateCopyrightHeader_$NOW"
logFile="$logBase.log"
logJobs="$logBase.jobs.log"
echo "logFile: " $logFile

# Check the argument count and content
if [ $# -eq 0 ]; then
    updateMode=false
    updateParam=""
elif [ $# -eq 1 ]; then
    if [ $1 == '--update' ] || [ $1 == '-u' ]; then
        updateMode=true
        updateParam='-u'
    else
        ExitScript 6  # Unrecognized parameter
    fi
else
    ExitScript 7 # Too many arguments
fi

# Check for the existance of the license template
if [ ! -f $HOOT_HOME/scripts/copyright/LicenseTemplate.txt ]; then
    ExitScript 1 # LicenseTemplate.txt not found
fi

# Run the checks in parallel
parallel --joblog $logJobs $HOOT_HOME/scripts/copyright/UpdateDirCopyrightHeaders.sh {} $logFile $updateParam ::: \
  $HOOT_HOME/hoot-core \
  $HOOT_HOME/hoot-core-test \
  $HOOT_HOME/hoot-services \
  $HOOT_HOME/tgs \
  $HOOT_HOME/hoot-js \
  $HOOT_HOME/hoot-rnd \
  $HOOT_HOME/tbs \
  $HOOT_HOME/hoot-test \
  $HOOT_HOME/hoot-cmd

# Get the first non-zero exit status from the parallel command
codes=`tail -n +2 $logJobs | awk '{print $7}' | grep "[1-9]" | head -n 1`
if [ -z "$codes" ]; then
    exitCode=$codes
fi

# Now examine the log file contents only if exitCode is 0.
if [[ $exitCode -eq 0 ]]; then
    if [ -f $logFile ]; then
        # The Python script wraps the file name with <> in its logging.
        # bash's regex engine does not support the .*? construct, therefore I went with this ...
        regex="Needs copyright header updated --> <([^<]+)>"
        counter=0
        if [ $updateMode == false ]; then
            echo "The following files need their copyright headers updated ..."
            while IFS='' read -r line || [[ -n "$line" ]]; do
                if [[ $line =~ $regex ]]; then
                    fileName="${BASH_REMATCH[1]}"
                    echo "    " $fileName
                    let counter++
                fi
            done < $logFile
            echo $counter "files need copyright header updating."
            if [ $counter -eq 0 ]; then
                # Nirvana! Perfect copyright headers in all checked files!
                exitCode=0
            else
                # This is not really an error situation, more a warning. 
                # There are files needing copyright header updating.
                exitCode=8
            fi
        else
            if [ $updateMode == true ]; then
                regex="Updating copyright header --> <([^<]+)>"
                echo "The following files had their copyright headers updated ..."
                while IFS='' read -r line || [[ -n "$line" ]]; do
                    if [[ $line =~ $regex ]]; then
                        fileName="${BASH_REMATCH[1]}"
                        echo "    " $fileName
                        let counter++
                    fi
                done < $logFile
                echo $counter "files were updated."

                # Whether copyright headers were updated or not, this is still a successful condition.
                exitCode=0
            else
                echo "Error: Unrecognized updateMode."
                echo    $updateMode  # should be 'true' or 'false'
                exitCode=9
            fi
        fi # if [ $updateMode == false ]

        # Check for errors that occurred in the Python script.
        regex="Error message --> <([^<]+)>"
        counter=0
        while IFS='' read -r line || [[ -n "$line" ]]; do
            if [[ $line =~ $regex ]]; then
                pythonErrorMessage="${BASH_REMATCH[1]}"
                #echo $line
                echo "    " $pythonErrorMessage
                let counter++
            fi
        done < $logFile
        echo $counter "Python errors were encountered."
        if [ $counter -ne 0 ]; then
            # Even if the exitCode were 0 up to this point, this error needs to be reviewed.
            exitCode=10
        fi
        echo $logFile "ready for review."
    else
        echo "Error: Log file not found"
        echo    $logFile
        exitCode=11
    fi
fi

ExitScript $exitCode
