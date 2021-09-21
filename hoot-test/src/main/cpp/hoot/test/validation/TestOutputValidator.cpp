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

# ifdef HOOT_HAVE_JOSM
#include <hoot/josm/validation/MapValidator.h>
# endif

// Qt
#include <QFileInfo>

namespace hoot
{

void TestOutputValidator::validate(
  const QString& testName, const QString& testOutputPath,
  const QString& goldValidationReportPath)
{
  // Make sure we have a base validation report to compare against.
  const QString goldValidationReportOffPath = goldValidationReportPath + ".off";
  QFileInfo goldValidationReport(goldValidationReportPath);
  QFileInfo goldValidationReportOff(goldValidationReportOffPath);
  if (!goldValidationReport.exists() && !goldValidationReportOff.exists())
  {
    throw TestConfigurationException("No gold validation report exists for test: " + testName);
  }
  // If we have an .off file present, skip validation.
  if (goldValidationReportOff.exists())
  {
    LOG_STATUS("Skipping validation for " << testName);
    return;
  }

  const QString outputValidationReportPath =
    goldValidationReport.dir().absolutePath() + "/validated-output-report";
  QFileInfo testOutput(testOutputPath);
  const QString validatedOutputPath = testOutput.dir().absolutePath() + "/ValidatedOutput.osm";

  // Write our validated output and validation report. The validated output is for debugging
  // purposes only and is not compared to anything.
  MapValidator validator;
  validator.setReportPath(outputValidationReportPath);
  validator.validate(QStringList(testOutputPath), validatedOutputPath);

  // Compare the validation reports.
  if (FileUtils::readFully(goldValidationReportPath) !=
      FileUtils::readFully(outputValidationReportPath))
  {
    CPPUNIT_ASSERT_MESSAGE(QString("Validation reports do not match").toStdString(), false);
  }
}

}
