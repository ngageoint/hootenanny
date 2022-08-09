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
 * @copyright Copyright (C) 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/elements/ElementDeduplicator.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/StringUtils.h>

// Qt
#include <QElapsedTimer>

namespace hoot
{

class DeDuplicateCmd : public BaseCommand
{
public:

  static QString className() { return "DeDuplicateCmd"; }

  DeDuplicateCmd() = default;

  QString getName() const override { return "de-duplicate"; }
  QString getDescription() const override
  { return "Removes duplicate features within a single map or between two maps (experimental)"; }
  QString getType() const override { return "advanced"; }

  int runSimple(QStringList& args) override
  {  
    QElapsedTimer timer;
    timer.start();

    bool favorMoreConnectedWays = false;
    if (args.contains("--favor-connected-ways"))
    {
      favorMoreConnectedWays = true;
      args.removeAll("--favor-connected-ways");
    }
    LOG_VARD(favorMoreConnectedWays);

    if (args.size() != 2 && args.size() != 4)
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw IllegalArgumentException(
        QString("%1 takes at least two or four parameters. You provided %2: %3")
          .arg(getName(), QString::number(args.size()), args.join(",")));
    }

    QString input1;
    QString input2;
    QString output1;
    QString output2;
    if (args.size() == 2)
    {
      input1 = args[0].trimmed();
      output1 = args[1].trimmed();
      LOG_STATUS(
        "De-duplicating ...:" << FileUtils::toLogFormat(input1, 25) << " to ..." <<
        FileUtils::toLogFormat(output1, 25) << "...");
    }
    else
    {
      input1 = args[0].trimmed();
      input2 = args[1].trimmed();
      output1 = args[2].trimmed();
      output2 = args[3].trimmed();
      LOG_STATUS(
        "De-duplicating ...:" << FileUtils::toLogFormat(input1, 25) << " to ..." <<
        FileUtils::toLogFormat(output1, 25) << " and ..." <<
        FileUtils::toLogFormat(input2, 25) << " to ..." << FileUtils::toLogFormat(output2, 25) <<
        "...");
    }
    LOG_VARD(input1);
    LOG_VARD(input2);
    LOG_VARD(output1);
    LOG_VARD(output2);

    ElementDeduplicator deduper;
    deduper.setFavorMoreConnectedWays(favorMoreConnectedWays);

    OsmMapPtr input1Map = std::make_shared<OsmMap>();
    IoUtils::loadMap(input1Map, input1, true, Status::Unknown1);
    input1Map->setName("map1");
    OsmMapPtr input2Map;
    if (input2.isEmpty())
    {
      deduper.dedupe(input1Map);
    }
    else
    {
      input2Map = std::make_shared<OsmMap>();
      IoUtils::loadMap(input2Map, input2, true, Status::Unknown2);
      input2Map->setName("map2");

      deduper.dedupe(input1Map, input2Map);
    }
    IoUtils::saveMap(input1Map, output1);
    if (!input2.isEmpty())
    {
      IoUtils::saveMap(input2Map, output2);
    }

    LOG_STATUS(
      "De-duplicated " <<
      StringUtils::formatLargeNumber(deduper.getMap1DuplicateTotalFeaturesRemoved()) <<
      " features from ..." << FileUtils::toLogFormat(input1) << " and wrote them to: ..." <<
      FileUtils::toLogFormat(output1, 25) << ".");
    LOG_STATUS(
      "\t" << StringUtils::formatLargeNumber(deduper.getMap1DuplicateNodesRemoved()) << " nodes");
    LOG_STATUS(
      "\t" << StringUtils::formatLargeNumber(deduper.getMap1DuplicateWaysRemoved()) << " ways");
    LOG_STATUS(
      "\t" << StringUtils::formatLargeNumber(deduper.getMap1DuplicateRelationsRemoved()) <<
      " relations");
    if (!input2.isEmpty())
    {
      LOG_STATUS(
        "De-duplicated " <<
        StringUtils::formatLargeNumber(deduper.getMap2DuplicateTotalFeaturesRemoved()) <<
        " features from ..." << FileUtils::toLogFormat(input2) << " and wrote them to: ..." <<
        FileUtils::toLogFormat(output2, 25) << ".");
      LOG_STATUS(
        "\t" << StringUtils::formatLargeNumber(deduper.getMap2DuplicateNodesRemoved()) << " nodes");
      LOG_STATUS(
        "\t" << StringUtils::formatLargeNumber(deduper.getMap2DuplicateWaysRemoved()) << " ways");
      LOG_STATUS(
        "\t" << StringUtils::formatLargeNumber(deduper.getMap2DuplicateRelationsRemoved()) <<
        " relations");
    }
    LOG_STATUS(
      "De-duplicated elements in " << StringUtils::millisecondsToDhms(timer.elapsed()) << " total.");

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, DeDuplicateCmd)

}
