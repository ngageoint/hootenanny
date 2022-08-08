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
 * @copyright Copyright (C) 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/StringUtils.h>

#include <hoot/josm/validation/MapValidator.h>

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

  int runSimple(QStringList& args) override
  {
    bool showAvailableValidatorsOnly = false;
    if (args.contains("--validators"))
    {
      showAvailableValidatorsOnly = true;
      args.removeAt(args.indexOf("--validators"));
    }

    if (showAvailableValidatorsOnly)
    {
      MapValidator::printValidators();
    }
    else
    {
      _validate(args);
    }

    return 0;
  }

private:

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
      validator.setReportPath(args.at(outputIndex + 1).trimmed());
      args.removeAt(outputIndex + 1);
      args.removeAt(outputIndex);
    }

    if (args.size() < 1)
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw IllegalArgumentException(QString("%1 takes at least one parameter.").arg(getName()));
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
};

HOOT_FACTORY_REGISTER(Command, ValidateCmd)

}
