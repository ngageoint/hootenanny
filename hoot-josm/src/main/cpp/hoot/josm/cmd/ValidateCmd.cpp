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
 * @copyright Copyright (C) 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/util/Factory.h>
//#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/StringUtils.h>

#include <hoot/josm/validation/MapValidator.h>
#include <hoot/josm/validation/ValidatedOutputCleaner.h>

// Qt
#include <QElapsedTimer>
#include <QDirIterator>

namespace hoot
{

/*
 * @todo think there's still problems here with certain combinations of --report-output, --output,
 * --separate-output, and --recursive
 */
class ValidateCmd : public BaseCommand
{
public:

  static QString className() { return "ValidateCmd"; }

  ValidateCmd() = default;

  QString getName() const override { return "validate"; }
  QString getType() const override { return "josm"; }
  QString getDescription() const override { return "Checks maps for validation errors"; }

  virtual int runSimple(QStringList& args) override
  {
    int standaloneOptionCount = 0;

    bool showAvailableValidatorsOnly = false;
    if (args.contains("--validators"))
    {
      showAvailableValidatorsOnly = true;
      args.removeAt(args.indexOf("--validators"));
      standaloneOptionCount++;
    }

    bool cleanValidatedOutputRecursively = false;
    if (args.contains("--cleanValidatedOutput"))
    {
      cleanValidatedOutputRecursively = true;
      args.removeAt(args.indexOf("--cleanValidatedOutput"));
      standaloneOptionCount++;
    }

//    bool runTests = false;
//    if (args.contains("--test"))
//    {
//      runTests = true;
//      args.removeAt(args.indexOf("--test"));
//      standaloneOptionCount++;
//    }

    if (standaloneOptionCount > 1)
    {
      throw IllegalArgumentException(
        QString("--validators, --cleanValidatedOutput, and --test must be specified ") +
        QString("independently of each other."));
    }

    if (showAvailableValidatorsOnly)
    {
      MapValidator::printValidators();
    }
    else if (cleanValidatedOutputRecursively)
    {
      if (args.size() != 1)
      {
        std::cout << getHelp() << std::endl << std::endl;
        throw HootException(
          QString(
            "%1 with --cleanValidatedOutput takes a single directory name as an input parameter.")
            .arg(getName()));
      }

      const int numDeleted = ValidatedOutputCleaner::clean(args[0]);
      LOG_STATUS("Deleted " << numDeleted << " validated outputs.");
    }
//    else if (runTests)
//    {
//      if (args.size() != 0)
//      {
//        std::cout << getHelp() << std::endl << std::endl;
//        throw HootException(QString("%1 with --test takes no input parameters.").arg(getName()));
//      }

//      return _runTests();
//    }
    else
    {
      _validate(args);
    }

    return 0;
  }

private:

  /**
   * @brief _validate TODO
   * @param args
   */
  void _validate(QStringList& args)
  {
    bool separateOutput = false;
    if (args.contains("--separate-output"))
    {
      separateOutput = true;
      args.removeAt(args.indexOf("--separate-output"));
    }

    QString output;
    if (args.contains("--output"))
    {
      const int outputIndex = args.indexOf("--output");
      output = args.at(outputIndex + 1).trimmed();
      args.removeAt(outputIndex + 1);
      args.removeAt(outputIndex);
    }

    if (separateOutput && !output.isEmpty())
    {
      throw IllegalArgumentException("--output and --separate-output cannot both be specified.");
    }

    bool recursive = false;
    const QStringList inputFilters = _parseRecursiveInputParameter(args, recursive);
    LOG_VARD(recursive);
    LOG_VARD(inputFilters);

    if (recursive && !separateOutput && output.isEmpty())
    {
      throw IllegalArgumentException(
        QString("--output must be specified when --recursive is specified and ") +
        QString("--separate-output is not specified."));
    }

    MapValidator validator;

    bool reportOutput = false;
    if (args.contains("--report-output"))
    {
      reportOutput = true;
      const int outputIndex = args.indexOf("--report-output");
      validator.setReportFile(args.at(outputIndex + 1).trimmed());
      args.removeAt(outputIndex + 1);
      args.removeAt(outputIndex);
    }

    if (args.size() < 1)
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw HootException(QString("%1 takes at least one parameter.").arg(getName()));
    }

    QElapsedTimer timer;
    timer.start();

    // Everything left is an input.
    QStringList inputs;
    if (!recursive)
    {
      inputs = IoUtils::expandInputs(args);
    }
    else
    {
      inputs = IoUtils::getSupportedInputsRecursively(args, inputFilters);
    }
    // Even though we have the cleaning routine available with --cleanValidatedOutput, we're still
    // going to clean out all previously validated files. Clearly will cause problems if anyone
    // want to validate files with "-validated" in the name.
    StringUtils::removeAllContaining(inputs, "-validated");
    LOG_VARD(inputs);

    const QString validationSummary = validator.validate(inputs, output);
    if (!reportOutput)
    {
      std::cout << validationSummary << std::endl;
    }

    LOG_STATUS(
      "Validate operation ran in " << StringUtils::millisecondsToDhms(timer.elapsed()) <<
      " total.");
  }

  /**
   * @brief _runTests TODO
   * @return
   */
//  static int _runTests()
//  {
//    QElapsedTimer timer;
//    timer.start();

//    // Currently, we support validating case test output only.
//    QStringList caseTestOutputs;
//    const bool anyTestOutputMissing = _getCaseTestOutputs(caseTestOutputs);
//    LOG_VARD(caseTestOutputs);
//    LOG_VARD(anyTestOutputMissing);

//    MapValidator().validate(caseTestOutputs);

//    QStringList caseTestValidationReports = caseTestOutputs;
//    caseTestValidationReports =
//      caseTestValidationReports.replaceInStrings("Output.osm", "validated-output-report");
//    LOG_VARD(caseTestValidationReports);

//    bool anyReportOutputMissing = false;
//    bool anyReportGoldFilesMissing = false;
//    bool anyReportDifferences = false;
//    for (int i = 0; i < caseTestValidationReports.size(); i++)
//    {
//      QString validationReportOutput = caseTestValidationReports.at(i);
//      if (!QFileInfo(validationReportOutput).exists())
//      {
//        anyReportOutputMissing = true;
//        continue;
//      }
//      const QString validationReportOutputContent = FileUtils::readFully(validationReportOutput);

//      const QString validationReportGold =
//        validationReportOutput.replace("validated-output-report", "validation-report");
//      if (!QFileInfo(validationReportGold).exists())
//      {
//        anyReportGoldFilesMissing = true;
//        continue;
//      }
//      const QString validationReportGoldContent = FileUtils::readFully(validationReportGold);

//      if (validationReportOutputContent != validationReportGoldContent)
//      {
//        LOG_ERROR(
//          "Report output does not match for " << validationReportGold << " and " <<
//          validationReportOutput << ".");
//        anyReportDifferences = true;
//      }
//    }
//    LOG_VARD(anyTestOutputMissing);
//    LOG_VARD(anyReportOutputMissing);
//    LOG_VARD(anyReportGoldFilesMissing);

//    LOG_STATUS(
//      "Validation tests ran in " << StringUtils::millisecondsToDhms(timer.elapsed()) <<
//      " total.");

//    if (anyTestOutputMissing || anyReportOutputMissing || anyReportGoldFilesMissing ||
//        anyReportDifferences)
//    {
//      return 1;
//    }
//    else
//    {
//      return 0;
//    }
//  }

  /**
   * @brief _getCaseTestOutputs TODO
   * @param outputs
   * @return
   */
//  static bool _getCaseTestOutputs(QStringList& outputs)
//  {
//    int logErrorCount = 0;
//    bool anyOutputsMissing = false;
//    QDirIterator itr(
//      "test-files/cases", QStringList(), QDir::AllDirs|QDir::Files, QDirIterator::Subdirectories);
//    while (itr.hasNext())
//    {
//      const QString path = itr.next();
//      if (QFileInfo(path).isDir())
//      {
//        QDirIterator itr2(path, QStringList("Output.osm"), QDir::Files);
//        const QString testOutputFile = itr2.next();
//        if (testOutputFile.isEmpty())
//        {
//          if (logErrorCount <= ConfigOptions().getLogWarnMessageLimit())
//          {
//            LOG_ERROR("No case test output found in directory: ..." << path.right(40) << ".");
//          }
//          anyOutputsMissing = true;
//          logErrorCount++;
//        }
//        else
//        {
//          outputs.append(testOutputFile);
//        }
//      }
//    }
//    outputs.sort();
//    return anyOutputsMissing;
//  }
};

HOOT_FACTORY_REGISTER(Command, ValidateCmd)

}
