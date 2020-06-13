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
#include <hoot/core/util/Factory.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/io/HootApiDb.h>

namespace hoot
{

class DbListMapsCmd : public BaseCommand
{
public:

  static std::string className() { return "hoot::DbListMapsCmd"; }

  DbListMapsCmd() = default;

  virtual QString getName() const override { return "db-list-maps"; }

  virtual QString getDescription() const override
  { return "Lists maps in the Hootenanny Web Services database available to a user"; }

  virtual int runSimple(QStringList& args) override
  {
    if (args.size() != 1)
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw HootException(QString("%1 takes one parameter.").arg(getName()));
    }

    HootApiDb mapReader;
    mapReader.open(args[0]);
    mapReader.setUserId(mapReader.getUserId(ConfigOptions().getApiDbEmail(), true));
    const QStringList mapNames = mapReader.selectMapNamesAvailableToCurrentUser();
    if (mapNames.size() == 0)
    {
      std::cout << "There are no maps available to the specified user in the Hootenanny Web Services database." << std::endl;
    }
    else
    {
      std::cout << "Available map layers:\n\n" << mapNames.join("\n") << std::endl;
    }
    mapReader.close();

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, DbListMapsCmd)

}
