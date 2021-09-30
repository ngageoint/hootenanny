/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */
#ifndef TEST_OUTPUT_VALIDATOR_H
#define TEST_OUTPUT_VALIDATOR_H

// Qt
#include <QString>

namespace hoot
{

/**
 * @brief The TestOutputValidator class validates test output.
 *
 * Currently, this is only used for case test output.
 */
class TestOutputValidator
{

public:

  TestOutputValidator() = default;
  ~TestOutputValidator() = default;

  /**
   * @brief validate validates test output
   * @param testName name of the test whose output is being validated
   * @param testOutputPath output directory for the test
   * @param goldValidationReportPath path to the baseline validation report for the test
   * @param suppressFailureDetail if true, detailed test failure information is not displayed
   * @param printValidationReportDiff if true, for failing tests prints the difference between the
   * baseline validation report and the test validation report output
   */
  static void validate(
    const QString& testName, const QString& testOutputPath,
    const QString& goldValidationReportPath, const bool suppressFailureDetail = false,
    const bool printValidationReportDiff = false);

private:

  /**
   * @brief _validateGoldReport validates the baseline report file used during test output
   * validation
   * @param testName name of the test whose output is being validated
   * @param goldValidationReportPath path to the baseline validation report for the test
   * @return true if the baseline report file is valid; false otherwise
   */
  static bool _validateGoldReport(
    const QString& testName, const QString& goldValidationReportPath);

  /**
   * @brief _printValidationReportDiff Prints the difference between two files
   * @param testName name of the current test
   * @param filePath1 first file to compare
   * @param filePath2 second file to compare
   * @param timeout how long to wait for the diff command to finish
   * @return difference string
   */
  static QString _printValidationReportDiff(
    const QString& testName, const QString& filePath1, const QString& filePath2, const int timeout);
};

}

#endif // TEST_OUTPUT_VALIDATOR_H
