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
            echo "    Warning: There are files that need their copyright headers updated"
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
        *)
            echo "    Error: Unknown"
            ;;
        esac
    fi
}

updateMode=false
exitCode=0

# Determine the log file name.
NOW=$(date +"%Y-%m-%d_%H:%M:%S")
logFile="$HOOT_HOME/updateCopyrightHeader_$NOW.log"
echo "logFile: " $logFile
#exit 0

# Updates all the copyright headers in all source directories.
for i in $HOOT_HOME/hoot-core/ $HOOT_HOME/hoot-cmd/ $HOOT_HOME/hoot-test $HOOT_HOME/hoot-js $HOOT_HOME/hoot-rnd $HOOT_HOME/hoot-services $HOOT_HOME/tgs $HOOT_HOME/pretty-pipes $HOOT_HOME/tbs
do
    #echo $i
    cd $i
    if [ $# -eq 0 ]; then
        # No arguments/parameters which means we are only CHECKING copyright headers.
        updateMode=false
        $HOOT_HOME/scripts/copyright/UpdateDirCopyrightHeaders.sh $logFile
        exitCode=$?
    else
        if [ $? -eq 1 ]; then
            if [ $1 == '--update' ] || [ $1 == '-u' ]; then
                updateMode=true
                $HOOT_HOME/scripts/copyright/UpdateDirCopyrightHeaders.sh $logFile $1
                exitCode=$?
            else
                # Unrecognized parameter.
                exitCode=6
            fi
        else
            # Too many arguments.
            exitCode=7
        fi
    fi
    echo $i  "exitCode:" $exitCode
    # Output the exit code message if non-zero.
    if [[ $exitCode -ne 0 ]]; then
        GetExitCodeText $exitCode
    fi
done

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

echo "exitCode:" $exitCode
GetExitCodeText $exitCode
exit $exitCode

