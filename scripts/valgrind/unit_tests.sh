#!/bin/bash

PARALLEL_JOBS=24

# Get all names from HootTest, remove all of the script test, remove the test count line
UNIT_TEST_LIST=`HootTest --all-names | grep -v "test-files/cmd" | tail -n +2`

DEBUG=""
if [ $# -gt 0 ]; then
  if [ "$1" == "--debug" ] || [ "$1" == "-d" ]; then
    DEBUG="yes"
  elif [ "$1" == "--single" ] && [ $# -eq 2 ]; then
    UNIT_TEST_LIST="$2"
  elif [ "$1" == "--include" ] && [ $# -eq 2 ]; then
    UNIT_TEST_LIST=`HootTest --all-names --include="$2" | grep -v "test-files/cmd" | tail -n +2`
  else
    echo "Unknown parameter"
  fi
fi

# Remove any paths and make test names into valid filenames
TEST_XML_LIST=`echo ${UNIT_TEST_LIST} | sed "s|${HOOT_HOME}/test-files/cases/||g" | sed 's|[/:]|_|g'`

merge_lists() {
  TEST_NAME=($1)
  LOG_NAME=($2)
  for ((i = 0; i < ${#TEST_NAME[@]}; ++i)); do
    cat << EOF
valgrind -s --xml=yes --num-callers=32 --trace-children=yes --vgdb=no \
--read-var-info=yes --read-inline-info=yes --fair-sched=try \
--suppressions=${HOOT_HOME}/valgrind.supp \
--xml-file=${HOOT_HOME}/tmp/valgrind2/${LOG_NAME[i]}.xml \
${HOOT_HOME}/bin/HootTest.bin --fatal --single ${TEST_NAME[i]} > /dev/null; echo -n '.'
EOF
  done
}

rm -rf $HOOT_HOME/tmp/valgrind
mkdir -p $HOOT_HOME/tmp/valgrind

merge_lists "$UNIT_TEST_LIST", "$TEST_XML_LIST" | parallel --jobs $PARALLEL_JOBS "{}"

exit
