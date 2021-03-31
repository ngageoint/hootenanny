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

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/criterion/WayHeadingVarianceCriterion.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/elements/MapProjector.h>

namespace hoot
{

class WayHeadingVarianceCriterionTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(WayHeadingVarianceCriterionTest);
  CPPUNIT_TEST(runPositiveTest);
  CPPUNIT_TEST(runNegativeTest);
  CPPUNIT_TEST(runBypassTest);
  CPPUNIT_TEST(runMissingMapTest);
  CPPUNIT_TEST(runMapNotInPlanarTest);
  CPPUNIT_TEST_SUITE_END();

public:

  WayHeadingVarianceCriterionTest() :
  HootTestFixture("test-files/", UNUSED_PATH)
  {
  }

  void runPositiveTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(map, _inputPath + "ToyTestA.osm");
    MapProjector::projectToPlanar(map);

    std::shared_ptr<WayHeadingVarianceCriterion> uut =
      std::make_shared<WayHeadingVarianceCriterion>(
        WayHeadingVarianceCriterion(22.5, NumericComparisonType::EqualTo, map));
    uut->setNumHistogramBins(16);
    uut->setSampleDistance(1.0);
    uut->setHeadingDelta(5.0);
    CPPUNIT_ASSERT(uut->isSatisfied(TestUtils::getElementWithNote(map, "2")));
  }

  void runNegativeTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(map, _inputPath + "ToyTestA.osm");
    MapProjector::projectToPlanar(map);

    std::shared_ptr<WayHeadingVarianceCriterion> uut =
      std::make_shared<WayHeadingVarianceCriterion>(
        WayHeadingVarianceCriterion(60.0, NumericComparisonType::GreaterThanOrEqualTo, map));
    uut->setNumHistogramBins(16);
    uut->setSampleDistance(1.0);
    uut->setHeadingDelta(5.0);
    CPPUNIT_ASSERT(!uut->isSatisfied(TestUtils::getElementWithNote(map, "2")));
  }

  void runBypassTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    WayPtr way = std::make_shared<Way>(Status::Unknown1, 1);
    map->addWay(way);

    // -1.0 for the length bypasses the crit; any way will work here
    std::shared_ptr<WayHeadingVarianceCriterion> uut =
      std::make_shared<WayHeadingVarianceCriterion>(-1.0, NumericComparisonType::EqualTo, map);
    CPPUNIT_ASSERT(uut->isSatisfied(way));
  }

  void runMissingMapTest()
  {
    WayPtr way = std::make_shared<Way>(Status::Unknown1, 1);

    std::shared_ptr<WayHeadingVarianceCriterion> uut =
      std::make_shared<WayHeadingVarianceCriterion>(1.0, NumericComparisonType::EqualTo, nullptr);
    QString exceptionMsg;
    try
    {
      uut->isSatisfied(way);
    }
    catch (const IllegalArgumentException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT_EQUAL(
      QString("WayHeadingVarianceCriterion requires a map.").toStdString(),
      exceptionMsg.toStdString());
  }

  void runMapNotInPlanarTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>(); // skip projecting map to planar
    WayPtr way = std::make_shared<Way>(Status::Unknown1, 1);
    map->addWay(way);

    std::shared_ptr<WayHeadingVarianceCriterion> uut =
      std::make_shared<WayHeadingVarianceCriterion>(1.0, NumericComparisonType::EqualTo, map);
    QString exceptionMsg;
    try
    {
      uut->isSatisfied(way);
    }
    catch (const IllegalArgumentException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT_EQUAL(
      QString("Map must be in planar coordinate system.").toStdString(),
      exceptionMsg.toStdString());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(WayHeadingVarianceCriterionTest, "quick");

}
