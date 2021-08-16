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
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/info/ApiEntityDisplayInfo.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/Log.h>
#include <hoot/josm/ops/JosmMapValidator.h>
#include <hoot/josm/ops/JosmMapCleaner.h>

namespace hoot
{

class ValidateCmd : public BaseCommand
{
public:

  static QString className() { return "hoot::ValidateCmd"; }

  ValidateCmd() = default;

  QString getName() const override { return "validate"; }
  QString getType() const override { return "josm"; }
  QString getDescription() const override { return "Checks map data for validation errors"; }

  virtual int runSimple(QStringList& args) override
  {
    bool showAvailableValidatorsOnly = false;
    if (args.contains("--validators"))
    {
      showAvailableValidatorsOnly = true;
      args.removeAt(args.indexOf("--validators"));
    }

    if (showAvailableValidatorsOnly)
    {
      _printJosmValidators();
    }
    else
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

        if (!IoUtils::isSupportedOutputFormat(output))
        {
          throw IllegalArgumentException("Invalid output location: " + output);
        }
      }

      if (separateOutput && !output.isEmpty())
      {
        throw IllegalArgumentException("--output and --separate-output cannot both be specified.");
      }

      QString reportOutput;
      if (args.contains("--report-output"))
      {
        const int outputIndex = args.indexOf("--report-output");
        reportOutput = args.at(outputIndex + 1).trimmed();
        args.removeAt(outputIndex + 1);
        args.removeAt(outputIndex);

        if (!IoUtils::isUrl(reportOutput))
        {
          // Write the output dir the report is in now so we don't get a nasty surprise at the end
          // of a long job that it can't be written.
          IoUtils::writeOutputDir(reportOutput);
        }
      }

      bool recursive = false;
      const QStringList inputFilters = _parseRecursiveInputParameter(args, recursive);
      LOG_VARD(recursive);
      LOG_VARD(inputFilters);

      if (args.size() < 1)
      {
        std::cout << getHelp() << std::endl << std::endl;
        throw HootException(QString("%1 takes at least one parameter.").arg(getName()));
      }

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
      LOG_VARD(inputs);

      QString validationSummary;
      if (!separateOutput)
      {
        // combines all inputs, writes them all to the same optional output, and displays a combined
        // validation summary for all the inputs
        validationSummary = _validate(inputs, output);
      }
      else
      {
        // writes a separate output for each input and prints the combined validation summary
        validationSummary = _validateSeparateOutput(inputs);
      }
      if (reportOutput.isEmpty())
      {
        std::cout << validationSummary << std::endl;
      }
      else
      {
        LOG_STATUS("Writing validation report summary to: ..." << reportOutput.left(25) << "...");
        FileUtils::writeFully(reportOutput, validationSummary);
      }
    }

    return 0;
  }

private:

  std::shared_ptr<JosmMapValidator> _validate(OsmMapPtr& map)
  {
    std::shared_ptr<JosmMapValidator> validator = std::make_shared<JosmMapValidator>();
    validator->setConfiguration(conf());
    LOG_STATUS(validator->getInitStatusMessage());
    validator->apply(map);
    return validator;
  }

  QString _validate(const QStringList& inputs, const QString& output)
  {      
    LOG_STATUS("Loading " << inputs.size() << " map(s)...");
    OsmMapPtr map = std::make_shared<OsmMap>();
    QString inputName;
    // We need the whole map in memory to validate it, so no point in trying to stream it in.
    if (inputs.size() == 1)
    {
      inputName = inputs.at(0);
      IoUtils::loadMap(map, inputs.at(0), true, Status::Unknown1);
    }
    else
    {
      // Avoid ID conflicts across multiple inputs.
      for (int i = 0; i < inputs.size(); i++)
      {
        inputName += inputs.at(i) + ";";
      }
      inputName.chop(1);
      IoUtils::loadMaps(map, inputs, false, Status::Unknown1);
    }

    LOG_STATUS("Validating combined map...");
    std::shared_ptr<JosmMapValidator> validator = _validate(map);

    if (!output.isEmpty())
    {
      MapProjector::projectToWgs84(map);
      IoUtils::saveMap(map, output);
    }

    return "Input: " + inputName + "\n" + validator->getSummary();
  }

  QString _validateSeparateOutput(const QStringList& inputs)
  {
    QString validationSummary;
    for (int i = 0; i < inputs.size(); i++)
    {
      const QString input = inputs.at(i);

      LOG_STATUS(
        "Loading map " << i + 1 << " of " << inputs.size() << ": ..." << input.right(25) <<
        "...");
      OsmMapPtr map = std::make_shared<OsmMap>();
      IoUtils::loadMap(map, input, true, Status::Unknown1);

      LOG_STATUS(
        "Validating map " << i + 1 << " of " << inputs.size() << ": ..." << input.right(25) <<
        "...");
      validationSummary += "Input: " + input + "\n";
      validationSummary += _validate(map)->getSummary() + "\n\n";

      // Write the output to a similarly named path as the input with some text appended to the
      // input name.
      const QString output = IoUtils::getOutputUrlFromInput(input, "-validated");
      LOG_STATUS(
        "Saving map " << i + 1 << " of " << inputs.size() << ": ..." << output.right(25) <<
        "...");
      MapProjector::projectToWgs84(map);
      IoUtils::saveMap(map, output);
    }
    return validationSummary.trimmed();
  }

  void _printJosmValidators()
  {
    JosmMapValidator validator;
    validator.setConfiguration(conf());
    _printValidatorOutput(validator.getValidatorDetail());
  }

  void _printValidatorOutput(const QMap<QString, QString>& validatorInfo)
  {
    for (QMap<QString, QString>::const_iterator itr = validatorInfo.begin();
         itr != validatorInfo.end(); ++itr)
    {
      const QString name = itr.key();
      const QString description = itr.value();
      const int indentAfterName = ApiEntityDisplayInfo::MAX_NAME_SIZE - name.size();
      const int indentAfterDescription = ApiEntityDisplayInfo::MAX_TYPE_SIZE - description.size();
      std::cout << name << QString(indentAfterName, ' ') << description <<
                   QString(indentAfterDescription, ' ') << std::endl;
    }
  }
};

HOOT_FACTORY_REGISTER(Command, ValidateCmd)

}
