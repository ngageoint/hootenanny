#!/bin/bash
set -e

# Should fail b/c TO_VALIDATE_1 is duplicated.

TO_VALIDATE_1=test-output/cmd/slow/ConflateCmdTest/output.osm
VALIDATION_REPORT_GOLD_1=test-files/cmd/slow/ConflateCmdTest/output-validation-report

TO_VALIDATE_1=test-output/cmd/slow/ConflateCmdTest/output.osm
VALIDATION_REPORT_GOLD_2=test-files/cmd/slow/ConflateCmdTest/output-validation-report
