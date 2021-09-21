#!/bin/bash
set -e

# Should fail b/c TO_VALIDATE doesn't end with an numeric index.
TO_VALIDATE=test-output/cmd/slow/ConflateCmdTest/output.osm
VALIDATION_REPORT_GOLD_1=test-files/cmd/slow/ConflateCmdTest/output-validation-report
