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
#include "TestOutputValidator.h"

// hoot
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>

#include <hoot/core/TestUtils.h>

#include <hoot/josm/validation/MapValidator.h>

// Qt
#include <QFileInfo>
#include <QProcess>
#include <QElapsedTimer>

namespace hoot
{

void TestOutputValidator::validate(
  const QString& testName, const QString& testOutputPath,
  const QString& goldValidationReportPath, const bool suppressFailureDetail,
  const bool printValidationReportDiff)
{
  LOG_VART(testName);
  LOG_VART(testOutputPath);
  LOG_VART(goldValidationReportPath);
  LOG_VART(printValidationReportDiff);

  // Make sure the baseline report file is valid first.
  if (!_validateGoldReport(testName, goldValidationReportPath))
  {
    return;
  }

  QFileInfo goldValidationReport(goldValidationReportPath);
  QFileInfo testOutput(testOutputPath);
  // e.g. For case tests, a gold validation report from the test input dir, "validation-report",
  // yields a test output validation report of "validated-output-report" in the output dir. All case
  // test gold validation report must be named exactly as "validation-report". For script tests, a
  // gold validation report from the test input dir, "output-validation-report", yields a test
  // output validation report of "output-validated-output-report". All script test gold validation
  // reports should have the base name of the output test file prefixed to "validation-report". The
  // difference between case and script test naming conventions is due to the fact that all case
  // tests only have a single output being validated, but a script test may have multiple outputs
  // being validated.
  const QString outputValidationReportPath =
    testOutput.dir().absolutePath() + "/" +
    goldValidationReport.completeBaseName().replace("validation-report", "validated-output-report");
  LOG_VART(outputValidationReportPath);
  // e.g. A "Output.osm" test output as input yields a "Output-validated.osm" validated test output.
  // The same behavior occurs for both case and script tests.
  const QString validatedOutputPath =
    testOutput.dir().absolutePath() + "/" +
    testOutput.completeBaseName().replace(
      testOutput.baseName(), testOutput.baseName() + "-validated.") + testOutput.completeSuffix();
  LOG_VART(validatedOutputPath);

  // Write our validated output and validation report. The validated output is for debugging
  // purposes only and is not compared to anything.
  MapValidator validator;
  validator.setReportPath(outputValidationReportPath);
  validator.validate(QStringList(testOutputPath), validatedOutputPath);

  // Compare the validation reports and fail if there are any differences.
  if (FileUtils::readFully(goldValidationReportPath) !=
      FileUtils::readFully(outputValidationReportPath))
  {
    QString msg = "Validation reports for test: " + testName + " do not match.";
    if (!suppressFailureDetail)
    {
      msg += "\n"
             "\n*************************\n"
             "  This can be resolved by reviewing the validation report output for correctness and then creating a new baseline.\n"
             "  Verify the changes in the output are satisfactory: \n"
             "    diff " + goldValidationReportPath + " " + outputValidationReportPath + "\n"
             "  Make a new baseline for the output:\n"
             "    mv " + outputValidationReportPath + " " + goldValidationReportPath + "\n"
             "*************************";
    }
    if (printValidationReportDiff)
    {
      msg +=
        "\n" +
        _printValidationReportDiff(
          testName, goldValidationReportPath, outputValidationReportPath,
          ConfigOptions().getTestScriptMaxExecTime())
          .trimmed();
    }
    CPPUNIT_ASSERT_MESSAGE(msg.toStdString(), false);
  }
}

bool TestOutputValidator::_validateGoldReport(
  const QString& testName, const QString& goldValidationReportPath)
{
  // Arbitrarily, we pick a standardized name for validation reports. Note this is currently used
  // with case tests only.
  QString goldValidationReportPathEndText = "validation-report";
  // Make sure we have a base validation report to compare against.
  if (!goldValidationReportPath.endsWith(goldValidationReportPathEndText))
  {
    QString errorMsg =
      QString("Validation report gold files for case tests should follow the naming ") +
      QString("convention: \"validation-report\".");
    throw TestConfigurationException(errorMsg);
  }
  const QString goldValidationReportOffPath = goldValidationReportPath + ".off";
  QFileInfo goldValidationReport(goldValidationReportPath);
  QFileInfo goldValidationReportOff(goldValidationReportOffPath);
  if (!goldValidationReport.exists() && !goldValidationReportOff.exists())
  {
    throw TestConfigurationException(
      "No gold validation report exists for test: " + testName + " at: " +
      goldValidationReportPath);
  }
  // If we have an .off file present, skip validation.
  if (goldValidationReportOff.exists())
  {
    LOG_STATUS("Skipping validation for " << testName << ".");
    return false;
  }

  return true;
}

QString TestOutputValidator::_printValidationReportDiff(
  const QString& testName, const QString& filePath1, const QString& filePath2, const int timeout)
{
  // Lifted this code from ScriptTest. It initially proved to be painful to try to refactor it out
  // of there for shared use...should probably eventually be done.

  QProcess diffProcess;
  diffProcess.start("diff", QStringList() << filePath1 << filePath2, QProcess::ReadOnly);

  while (diffProcess.waitForStarted(500) == false)
  {
    LOG_WARN("Waiting for diff process to start for: " + testName);
  }

  const bool scriptTimeOutSpecified = timeout != -1;
  bool first = true;
  QElapsedTimer timer;
  timer.start();
  while (diffProcess.waitForFinished(timeout * 1000) == false)
  {
    if (first)
    {
      // Throw an endl in there so the dots in the test list don't look funny.
      std::cout << std::endl;
      LOG_WARN("Waiting for diff process to finish for: " + testName);
      first = false;
    }

    // If the process hangs, this will allow us to get out.
    const qint64 timeElapsedSeconds = timer.elapsed() / 1000;
    if (scriptTimeOutSpecified && timeElapsedSeconds >= timeout)
    {
      LOG_ERROR(
        "Forcefully ending diff command for: " << testName << " after " << timeElapsedSeconds <<
        " seconds.");
      break;
    }
  }

 return
   "Validation report diff: \n" + QString::fromUtf8(diffProcess.readAllStandardOutput()) + "\n" +
   QString::fromUtf8(diffProcess.readAllStandardError());
}

}
