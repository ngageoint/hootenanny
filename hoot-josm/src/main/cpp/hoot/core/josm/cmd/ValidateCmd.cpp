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

      if (args.size() < 1)
      {
        std::cout << getHelp() << std::endl << std::endl;
        throw HootException(QString("%1 takes at least one parameter.").arg(getName()));
      }

      // Everything left is an input.
      const QStringList inputs = args;

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

      JosmMapValidator validator;
      validator.setConfiguration(conf());
      LOG_STATUS(validator.getInitStatusMessage());
      validator.apply(map);
      LOG_INFO(validator.getCompletedStatusMessage());

      if (!output.isEmpty())
      {
        MapProjector::projectToWgs84(map);
        IoUtils::saveMap(map, output);
      }

      std::cout << validator.getSummary() << std::endl;
    }

    return 0;
  }

private:

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

