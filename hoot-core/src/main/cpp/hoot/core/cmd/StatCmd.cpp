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
 * @copyright Copyright (C) 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/info/StatCalculator.h>

namespace hoot
{

class StatCmd : public BaseCommand
{
public:

  static QString className() { return "hoot::StatCmd"; }

  StatCmd() = default;

  QString getName() const override { return "stat"; }
  QString getDescription() const override { return "Calculates a single statistic for a map"; }

  int runSimple(QStringList& args) override
  {
    if (args.size() < 2 || args.size() > 3)
    {
      LOG_VAR(args);
      std::cout << getHelp() << std::endl << std::endl;
      throw HootException(QString("%1 takes two or three parameters.").arg(getName()));
    }

    const QString input = args[0].trimmed();
    QString visClassName = args[1].trimmed();
    QString statType = "total";
    if (args.size() == 3)
    {
      statType = args[2].trimmed();
    }

    const double stat = StatCalculator().calculateStat(input, visClassName, statType);
    // see note in CountCmd about the preceding endline
    std::cout << std::endl << "Calculated statistic: " <<
                 QString::number(stat, 'g', 3) << std::endl;

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, StatCmd)

}

