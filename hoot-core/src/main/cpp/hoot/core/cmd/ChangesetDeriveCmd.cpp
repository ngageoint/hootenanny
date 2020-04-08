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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/algorithms/changeset/ChangesetCreator.h>
#include <hoot/core/cmd/BoundedCommand.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/io/ChangesetStatsFormat.h>

// Qt
#include <QFileInfo>

namespace hoot
{

/**
 * Derives a set of changes given one or two map inputs
 */
class ChangesetDeriveCmd : public BoundedCommand
{
public:

  static std::string className() { return "hoot::ChangesetDeriveCmd"; }

  ChangesetDeriveCmd() {}

  virtual QString getName() const override { return "changeset-derive"; }

  virtual QString getDescription() const override
  { return "Creates an OSM changeset representing the difference between two maps"; }

  virtual int runSimple(QStringList& args) override
  {
    BoundedCommand::runSimple(args);

    bool printStats = false;
    QString outputStatsFile;
    if (args.contains("--stats"))
    {
      printStats = true;
      const int statsIndex = args.indexOf("--stats");
      if (!args[statsIndex + 1].startsWith("--"))
      {
        outputStatsFile = args[statsIndex + 1];

        QFileInfo statsFileInfo(outputStatsFile);
        if (!statsFileInfo.completeSuffix().isEmpty())
        {
          /*ChangesetStatsFormat format =*/
            ChangesetStatsFormat::fromString(statsFileInfo.completeSuffix());
        }
        args.removeAll(outputStatsFile);
      }
      args.removeAll("--stats");
    }
    LOG_VARD(printStats);
    LOG_VARD(outputStatsFile);

    if (args.size() < 3 || args.size() > 4)
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw HootException(QString("%1 takes three or four parameters.").arg(getName()));
    }

    const QString input1 = args[0];
    const QString input2 = args[1];
    const QString output = args[2];
    QString osmApiDbUrl;
    if (output.endsWith(".osc.sql"))
    {
      if (args.size() != 4)
      {
        std::cout << getHelp() << std::endl << std::endl;
        throw IllegalArgumentException(
          QString("%1 with SQL changeset output takes four parameters.").arg(getName()));
      }
      osmApiDbUrl = args[3];
    }
    else if (args.size() > 3)
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw IllegalArgumentException(
        QString("%1 with output: " + output + " takes three parameters.").arg(getName()));
    }

    ChangesetCreator(printStats, outputStatsFile, osmApiDbUrl).create(output, input1, input2);

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, ChangesetDeriveCmd)

}


