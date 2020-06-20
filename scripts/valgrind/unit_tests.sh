#!/bin/bash

PARALLEL_JOBS=16

# Get all names from HootTest, remove all of the script test, remove the test count line
UNIT_TEST_LIST=`HootTest --all-names | grep -v "test-files/cmd" | tail -n +2`

print_usage() {
  cat << EOF
Run Hootenanny unit tests (minus the script tests) through Valgrind and generate XML output
logs that can be opened in QtCreator

  usage: unit_test.sh [OPTIONS]
  options:
    -d --debug                      Run script in debug mode, output test names
    -s --single <test>              Run script on single unit test i.e. HootTest --single <test>
    -i --include <pattern>          Run script against unit tests that match <pattern> i.e. HootTest --include=<pattern>
    -l --list <tests>               Run a quoted, space separated list of jobs
    -g --generate-suppression-logs  Generate suppresion logs instead of XML
    -j --jobs <count>               Run <count> number of jobs in parallel, default is 16
    -u --update                     Update the current logs (don't delete directory, overwrite existing)
EOF

  exit
}

DEBUG="no"
GENERATE_SUPPRESSION_LOGS="no"
UPDATE="no"
while [[ $# -gt 0 ]]
do
  if [[ ("$1" == "--debug" || "$1" == "-d") ]]; then
    DEBUG="yes"
    shift
  elif [[ ("$1" == "--single" || "$1" == "-s") && $# -ge 2 ]]; then
    UNIT_TEST_LIST="$2"
    PARALLEL_JOBS=1
    shift; shift
  elif [[ ("$1" == "--include" || "$1" == "-i")  &&  $# -ge 2 ]]; then
    UNIT_TEST_LIST=`HootTest --all-names --include="$2" | grep -v "test-files/cmd" | tail -n +2`
    shift; shift
  elif [[ ("$1" == "--list" || "$1" == "-l") && $# -ge 2 ]]; then
    UNIT_TEST_LIST=$2
    shift; shift
  elif [[ ("$1" == "--generate-suppression-logs" || "$1" == "-g") ]]; then
    GENERATE_SUPPRESSION_LOGS="yes"
    shift
  elif [[ ("$1" == "--help" || "$1" == "-h") ]]; then
    print_usage
  elif [[ ("$1" == "--jobs" || "$1" == "-j") && $# -ge 2 ]]; then
    PARALLEL_JOBS="$2"
    shift; shift
  elif [[ ("$1" == "--update" || "$1" == "-u") ]]; then
    UPDATE="yes"
    shift
  else
    echo "Unknown parameter"
    print_usage
  fi
done

# Remove any paths and make test names into valid filenames
TEST_XML_LIST=`echo ${UNIT_TEST_LIST} | sed "s|${HOOT_HOME}/test-files/cases/||g" | sed 's|[/:]|_|g'`

# Output the number of tests to run
TEST_COUNT_LIST=($UNIT_TEST_LIST)
echo -n "Running ${#TEST_COUNT_LIST[@]} test"
if [[ ${#TEST_COUNT_LIST[@]} -gt 1 ]]; then
  echo -n "s"
fi
if [[ ${PARALLEL_JOBS} -gt 1 ]]; then
  echo -n " in ${PARALLEL_JOBS} jobs"
fi
echo ":"

# Valgrind common parameters
NUM_CALLERS="--num-callers=${PARALLEL_JOBS}"
SUPPRESSIONS="--suppressions=${HOOT_HOME}/scripts/valgrind/hoot_valgrind.supp"
ERROR_LIMIT="--error-limit=no"
USE_XML="--xml=yes"
READ_INLINE="--read-inline-info=yes"
USE_FAIR_SCHED="--fair-sched=try"
DEMANGLE="--demangle=no"

# Function to merge the two lists into one list of valgrind commands
merge_lists() {
  TEST_NAME=($1)
  LOG_NAME=($2)
  # Iterate all of the tests
  for ((i = 0; i < ${#TEST_NAME[@]}; ++i)); do
    # Setup progress 'echo' command
    if [[ "$DEBUG" == "yes" ]]; then
      ECHO_CMD=" echo '${TEST_NAME[i]}'"
    else
      ECHO_CMD=" echo -n '.'"
    fi
    # Output the command with all options
    if [[ "$GENERATE_SUPPRESSION_LOGS" == "no" ]]; then
      cat << EOF
valgrind -s --trace-children=yes --vgdb=no --track-origins=yes \
${DEMANGLE} \
${NUM_CALLERS} \
${ERROR_LIMIT} \
${SUPPRESSIONS} \
${READ_INLINE} \
${USE_FAIR_SCHED} \
${USE_XML} \
${READ_VAR} \
--xml-file=${HOOT_HOME}/tmp/valgrind/${LOG_NAME[i]}.xml \
${HOOT_HOME}/bin/HootTest.bin --fatal --single ${TEST_NAME[i]} > /dev/null; \
${ECHO_CMD}
EOF
    else
      cat << EOF
valgrind --gen-suppressions=all \
${DEMANGLE} \
${NUM_CALLERS} \
${ERROR_LIMIT} \
${SUPPRESSIONS} \
--log-file=${HOOT_HOME}/tmp/valgrind/${LOG_NAME[i]}.log \
${HOOT_HOME}/bin/HootTest.bin --fatal --single ${TEST_NAME[i]} > /dev/null; \
cat ${HOOT_HOME}/tmp/valgrind/${LOG_NAME[i]}.log | \
${HOOT_HOME}/scripts/valgrind/grindmerge.pl > ${HOOT_HOME}/tmp/valgrind/${LOG_NAME[i]}.supp > /dev/null 2>&1; \
${ECHO_CMD}
EOF
    fi
  done
}

# Cleanout and create the valgrind output directory when not updating
if [[ "$UPDATE" == "no" ]]; then
  rm -f $HOOT_HOME/tmp/valgrind/*
  mkdir -p $HOOT_HOME/tmp/valgrind
fi

# Run the merged list of valgrind commands in parallel
merge_lists "$UNIT_TEST_LIST" "$TEST_XML_LIST" | parallel --jobs $PARALLEL_JOBS "{}"

if [[ "$DEBUG" == "no" ]]; then
  echo ""
fi
