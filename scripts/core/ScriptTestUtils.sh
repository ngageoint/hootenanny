#!/bin/bash
set -e

# Anything utility functions that need to be called by multiple script tests can be added here. If 
# you create any script-wide vars in here, don't use any var names here that might conflict with 
# vars commonly used in script tests. e.g. "CONFIG", "LOG_LEVEL".

# @brief validateTestOutput runs validation on the specified file if test output validation is 
# enabled and compares the resulting validation report to the specified baseline report. A validated 
# file is output for debugging purposes.
# @param toValidate test output file to validate under the test-output directory
# @param validationReportOut validation report output file under the test-output directory
# @param validatedOut validated output file; same as input file to validate but with added tags 
# identifying features with validation errors
# @param goldValidationReport baseline validation report to compare the test generated validation 
# report with
function validateTestOutput()
{
  if [ -f "$HOOT_HOME/test-output/test-validation-enabled" ]; then
    #echo "Running validation..."
    toValidate="$1"
    validationReportOut="$2"
    validatedOut="$3"
    goldValidationReport="$4"
    hoot validate --warn -C Testing.conf $toValidate --report-output $validationReportOut --output $validatedOut
    diff $goldValidationReport $validationReportOut
  fi
}
