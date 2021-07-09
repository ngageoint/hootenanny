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
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */

//  Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/WayAverager.h>
#include <hoot/core/elements/MapUtils.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/ops/CopyMapSubsetOp.h>

namespace hoot
{

class WayAveragerTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(WayAveragerTest);
  CPPUNIT_TEST(runSimpleTest);
  CPPUNIT_TEST_SUITE_END();

public:

  WayAveragerTest() :
  HootTestFixture(
    "test-files/algorithms/WayAveragerTest/", "test-output/algorithms/WayAveragerTest/")
  {
    setResetType(ResetAll);
  }

  void runSimpleTest()
  {
    OsmMapPtr sourceMap = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(sourceMap, _inputPath + "input1.osm", false, Status::Unknown1);
    OsmMapReaderFactory::read(sourceMap, _inputPath + "input2.osm", false, Status::Unknown2);

    OsmMapPtr targetMap = std::make_shared<OsmMap>();
    _replaceWithAveragedWay(
      sourceMap, "{7dec13af-4519-426b-a007-a392a3e8710c}", "{46030cab-b4e4-4a65-952f-99bb09de439f}",
      targetMap);
    _replaceWithAveragedWay(
      sourceMap, "{ea7d50c0-52cc-489b-861a-3c26ff3eb86b}", "{c6523954-dc6a-4df5-84d6-10e059cf63a3}",
      targetMap);

    OsmMapWriterFactory::write(targetMap, _outputPath + "runSimpleTestOut.osm");
    HOOT_FILE_EQUALS(_inputPath + "runSimpleTestOut.osm", _outputPath + "runSimpleTestOut.osm");
  }

private:

  void _replaceWithAveragedWay(
    OsmMapPtr& sourceMap, const QString& input1Uuid, const QString& input2Uuid,
    OsmMapPtr& targetMap)
  {
    WayPtr way1 =
      std::dynamic_pointer_cast<Way>(MapUtils::getFirstElementWithTag(sourceMap, "uuid", input1Uuid));
    way1->setTag("note", "average input 1");
    WayPtr way2 =
      std::dynamic_pointer_cast<Way>(MapUtils::getFirstElementWithTag(sourceMap, "uuid", input2Uuid));
    way2->setTag("note", "average input 2");
    std::set<ElementId> elementIds;
    elementIds.insert(way1->getElementId());
    elementIds.insert(way2->getElementId());
    std::shared_ptr<CopyMapSubsetOp> mapCopier(new CopyMapSubsetOp(sourceMap, elementIds));
    mapCopier->apply(targetMap);

    /*WayPtr averagedWay =*/ WayAverager::replaceWaysWithAveragedWay(targetMap, way1, way2);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(WayAveragerTest, "quick");

}
