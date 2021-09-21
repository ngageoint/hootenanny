#!/bin/bash
set -e

# Should fail b/c VALIDATION_REPORT_GOLD doesn't end with an numeric index.
TO_VALIDATE_1=test-output/cmd/slow/ConflateCmdTest/output.osm
VALIDATION_REPORT_GOLD=test-files/cmd/slow/ConflateCmdTest/output-validation-report
