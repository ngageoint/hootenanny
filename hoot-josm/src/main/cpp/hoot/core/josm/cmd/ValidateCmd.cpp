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
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
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
    if (args.size() < 1 || args.size() > 2)
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw HootException(QString("%1 takes one or two parameters.").arg(getName()));
    }

    if (args.size() == 1)
    {
      if (!args.contains("--available-validators"))
      {
        throw IllegalArgumentException(
          "When the validate command is called with one parameter, the parameter must be "
          "'--available-validators'.");
      }

      _printJosmValidators();
    }
    else
    {
      const QString input = args[0];
      const QString output = args[1];

      OsmMapPtr map(new OsmMap());
      IoUtils::loadMap(map, input, true, Status::Unknown1);

      JosmMapValidator validator;
      validator.setConfiguration(conf());
      LOG_INFO(validator.getInitStatusMessage());
      validator.apply(map);
      LOG_INFO(validator.getCompletedStatusMessage());

      MapProjector::projectToWgs84(map);
      IoUtils::saveMap(map, output);

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

