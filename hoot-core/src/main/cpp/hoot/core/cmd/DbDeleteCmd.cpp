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
 * @copyright Copyright (C) 2012, 2013, 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/io/HootApiDbWriter.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/StringUtils.h>

// Qt
#include <QElapsedTimer>

using namespace std;

namespace hoot
{

class DbDeleteCmd : public BaseCommand
{
public:

  static QString className() { return "DbDeleteCmd"; }

  DbDeleteCmd() = default;

  QString getName() const override { return "db-delete"; }
  QString getDescription() const override
  { return "Deletes a map from the Hootenanny Web Services database"; }
  QString getType() const override { return "advanced"; }

  int runSimple(QStringList& args) override
  {
    if (args.size() < 1)
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw IllegalArgumentException(
        QString("%1 takes at least one parameter. You provided %2: %3")
          .arg(getName())
          .arg(args.size())
          .arg(args.join(",")));
    }

    QElapsedTimer timer;
    timer.start();

    const QStringList inputs = args;
    for (int i = 0; i < inputs.size(); i++)
    {
      LOG_STATUS("Deleting ..." << FileUtils::toLogFormat(inputs.at(i), 25) << "...");
      HootApiDbWriter().deleteMap(inputs.at(i));
    }

    LOG_STATUS(
      "Map(s) deleted in " << StringUtils::millisecondsToDhms(timer.elapsed()) << " total.");

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, DbDeleteCmd)

}
