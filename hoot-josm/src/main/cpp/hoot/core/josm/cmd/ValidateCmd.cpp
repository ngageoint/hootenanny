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
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/josm/ops/JosmMapValidator.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/info/ApiEntityDisplayInfo.h>

namespace hoot
{

class ValidateCmd : public BaseCommand
{
public:

  static QString className() { return "hoot::ValidateCmd"; }

  ValidateCmd() = default;

  virtual QString getName() const override { return "validate"; }
  virtual QString getType() const { return "josm"; }
  virtual QString getDescription() const override
  { return "Checks map data for validation errors"; }

  virtual int runSimple(QStringList& args) override
  {
    bool showAvailableValidatorsOnly = false;
    if (args.contains("--available-validators"))
    {
      showAvailableValidatorsOnly = true;
      args.removeAt(args.indexOf("--available-validators"));
    }

    bool separateOutput = false;
    if (args.contains("--separate-output"))
    {
      separateOutput = true;
      args.removeAt(args.indexOf("--separate-output"));
    }

    bool recursive = false;
    const QStringList inputFilters = _parseRecursiveInputParameter(args, recursive);
    LOG_VARD(recursive);
    LOG_VARD(inputFilters);

    if (showAvailableValidatorsOnly)
    {
      _printJosmValidators();
    }
    else
    {
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

      if (args.size() < 1)
      {
        std::cout << getHelp() << std::endl << std::endl;
        throw HootException(QString("%1 takes at least one parameter.").arg(getName()));
      }

      // Everything left is an input.
      QStringList inputs;
      if (!recursive)
      {
        inputs = args;
      }
      else
      {
        inputs = IoUtils::getSupportedInputsRecursively(args, inputFilters);
      }
      LOG_VARD(inputs);

      if (!separateOutput)
      {
        // combines all inputs, generates their validation summary, and optionally writes them all
        // to the same output
        std::cout << _validate(inputs, output)->getSummary() << std::endl;
      }
      else
      {
        // writes a separate output for each input
        _validateSeparateOutput(inputs);
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
    LOG_INFO(validator->getCompletedStatusMessage());
    return validator;
  }

  std::shared_ptr<JosmMapValidator> _validate(const QStringList& inputs, const QString& output)
  {
    OsmMapPtr map(new OsmMap());
    if (inputs.size() == 1)
    {
      IoUtils::loadMap(map, inputs.at(0), true, Status::Unknown1);
    }
    else
    {
      // Avoid ID conflicts across multiple inputs.
      IoUtils::loadMaps(map, inputs, false, Status::Unknown1);
    }

    std::shared_ptr<JosmMapValidator> validator = _validate(map);

    if (!output.isEmpty())
    {
      MapProjector::projectToWgs84(map);
      IoUtils::saveMap(map, output);
    }

    return validator;
  }

  void _validateSeparateOutput(const QStringList& inputs)
  {
    for (int i = 0; i < inputs.size(); i++)
    {
      const QString input = inputs.at(i);

      OsmMapPtr map(new OsmMap());
      IoUtils::loadMap(map, input, true, Status::Unknown1);

      /*std::shared_ptr<JosmMapValidator> validator =*/ _validate(map);

      // Write the output to a similarly named path as the input with some text appended to the
      // input name.
      const QString output = _getSeparateOutputUrl(input, "-validated");
      MapProjector::projectToWgs84(map);
      IoUtils::saveMap(map, output);
    }
  }

  void _printJosmValidators()
  {
    const QMap<QString, QString> validators =
      JosmMapValidator().getAvailableValidatorsWithDescription();
    for (QMap<QString, QString>::const_iterator itr = validators.begin(); itr != validators.end();
         ++itr)
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
