#!/bin/bash
set -e

# Should fail b/c VALIDATION_REPORT_GOLD_1 is duplicated.

TO_VALIDATE_1=test-output/cmd/slow/ConflateCmdTest/output.osm
VALIDATION_REPORT_GOLD_1=test-files/cmd/slow/ConflateCmdTest/output-validation-report

TO_VALIDATE_2=test-output/cmd/slow/ConflateCmdTest/output.osm
VALIDATION_REPORT_GOLD_1=test-files/cmd/slow/ConflateCmdTest/output-validation-report
