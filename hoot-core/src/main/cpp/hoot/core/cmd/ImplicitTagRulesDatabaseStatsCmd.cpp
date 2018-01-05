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
 * @copyright Copyright (C) 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/io/ImplicitTagRulesSqliteReader.h>

namespace hoot
{

/**
 *
 */
class ImplicitTagRulesDatabaseStatsCmd : public BaseCommand
{
public:

  static std::string className() { return "hoot::ImplicitTagRulesDatabaseStatsCmd"; }

  virtual QString getName() const { return "implicit-tagging-rules-database-stats"; }

  virtual int runSimple(QStringList args)
  {
    if (args.size() != 1)
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw HootException(QString("%1 takes one parameters.").arg(getName()));
    }

    ImplicitTagRulesSqliteReader dbReader;
    dbReader.open(args[0].trimmed());
    dbReader.printStats();
    dbReader.close();

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, ImplicitTagRulesDatabaseStatsCmd)

}
