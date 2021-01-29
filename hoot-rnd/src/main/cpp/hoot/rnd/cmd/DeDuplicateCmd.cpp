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
 * @copyright Copyright (C) 2020, 2021 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/elements/ElementDeduplicator.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/io/IoUtils.h>

// Qt
#include <QElapsedTimer>

namespace hoot
{

/*
 * @todo needs command line test
 */
class DeDuplicateCmd : public BaseCommand
{
public:

  static QString className() { return "hoot::DeDuplicateCmd"; }

  DeDuplicateCmd()
  {
  }

  virtual QString getName() const override { return "de-duplicate"; }

  virtual QString getDescription() const override
  { return "Removes duplicate features within a single map or between two maps (experimental)"; }

  virtual QString getType() const { return "rnd"; }

  virtual int runSimple(QStringList& args) override
  {  
    QElapsedTimer timer;
    timer.start();

    bool dedupeIntraMap = true;
    if (args.contains("--skip-intra-map"))
    {
      dedupeIntraMap = false;
      args.removeAll("--skip-intra-map");
    }
    LOG_VARD(dedupeIntraMap);

    bool dedupeNodes = true;
    if (args.contains("--skip-nodes"))
    {
      dedupeNodes = false;
      args.removeAll("--skip-nodes");
    }
    LOG_VARD(dedupeNodes);

    bool dedupeWays = true;
    if (args.contains("--skip-ways"))
    {
      dedupeWays = false;
      args.removeAll("--skip-ways");
    }
    LOG_VARD(dedupeWays);

    bool dedupeRelations = true;
    if (args.contains("--skip-relations"))
    {
      dedupeRelations = false;
      args.removeAll("--skip-relations");
    }
    LOG_VARD(dedupeRelations);

    bool favorMoreConnectedWays = false;
    if (args.contains("--favor-connected-ways"))
    {
      favorMoreConnectedWays = true;
      args.removeAll("--favor-connected-ways");
    }
    LOG_VARD(favorMoreConnectedWays);

    ElementCriterionPtr nodeCrit = _parseFilter("--node-filter", args);
    ElementCriterionPtr wayCrit = _parseFilter("--way-filter", args);
    ElementCriterionPtr relationCrit = _parseFilter("--relation-filter", args);

    if (args.size() != 2 && args.size() != 4)
    {
      LOG_VAR(args);
      std::cout << getHelp() << std::endl << std::endl;
      throw HootException(QString("%1 takes two or four parameters.").arg(getName()));
    }

    QString input1;
    QString input2;
    QString output1;
    QString output2;
    if (args.size() == 2)
    {
      if (!dedupeIntraMap)
      {
        LOG_WARN("--skip-intra-map option ignored for single input map.");
      }

      input1 = args[0].trimmed();
      output1 = args[1].trimmed();
      LOG_STATUS(
        "De-duplicating ...:" << input1.right(25) << " to ..." << output1.right(25) << "...");
    }
    else
    {
      input1 = args[0].trimmed();
      input2 = args[1].trimmed();
      output1 = args[2].trimmed();
      output2 = args[3].trimmed();
      LOG_STATUS(
        "De-duplicating ...:" << input1.right(25) << " to ..." << output1.right(25) << " and ..." <<
        input2.right(25) << " to ..." << output2.right(25) << "...");
    }
    LOG_VARD(input1);
    LOG_VARD(input2);
    LOG_VARD(output1);
    LOG_VARD(output2);

    ElementDeduplicator deduper;
    deduper.setDedupeIntraMap(dedupeIntraMap);
    deduper.setDedupeNodes(dedupeNodes);
    deduper.setDedupeWays(dedupeWays);
    deduper.setDedupeRelations(dedupeRelations);
    deduper.setNodeCriterion(nodeCrit);
    deduper.setWayCriterion(wayCrit);
    deduper.setRelationCriterion(relationCrit);
    deduper.setFavorMoreConnectedWays(favorMoreConnectedWays);

    OsmMapPtr input1Map(new OsmMap());
    IoUtils::loadMap(input1Map, input1, true, Status::Unknown1);
    input1Map->setName("map1");
    OsmMapPtr input2Map;
    if (input2.isEmpty())
    {
      deduper.dedupe(input1Map);
    }
    else
    {
      input2Map.reset(new OsmMap());
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
      " features from ..." << input1 << " and wrote them to: ..." << output1.right(25) << ".");
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
        " features from ..." << input2 << " and wrote them to: ..." << output2.right(25) << ".");
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

private:

  ElementCriterionPtr _parseFilter(const QString& optionName, QStringList& args)
  {
    ElementCriterionPtr crit;
    if (args.contains(optionName))
    {
      const int optionNameIndex = args.indexOf(optionName);
      QString critClassName = args.at(optionNameIndex + 1).trimmed();
      args.removeAt(optionNameIndex + 1);
      args.removeAt(optionNameIndex);

      try
      {
        crit.reset(Factory::getInstance().constructObject<ElementCriterion>(critClassName));
      }
      catch (const boost::bad_any_cast&)
      {
        throw IllegalArgumentException("Invalid criterion: " + critClassName);
      }
    }
    if (crit)
    {
      LOG_DEBUG(optionName << ": " << crit);
    }
    return crit;
  }
};

HOOT_FACTORY_REGISTER(Command, DeDuplicateCmd)

}
