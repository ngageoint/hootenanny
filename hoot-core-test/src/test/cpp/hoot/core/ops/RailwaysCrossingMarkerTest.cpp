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
#include <hoot/core/elements/MapUtils.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/ops/RailwaysCrossingMarker.h>

namespace hoot
{

class RailwaysCrossingMarkerTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(RailwaysCrossingMarkerTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST(runTagExcludeTest);
  CPPUNIT_TEST(runPreventIntraDatasetMarkingTest);
  CPPUNIT_TEST_SUITE_END();

public:

  RailwaysCrossingMarkerTest() :
  HootTestFixture(
    "test-files/ops/RailwaysCrossingMarkerTest/", "test-output/ops/RailwaysCrossingMarkerTest/")
  {
  }

  void runBasicTest()
  {
    // The basic config will create a review for any crossing rail pairs. One review should be
    // generated.

    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(map, _inputPath + "input.osm", false, Status::Unknown1);

    RailwaysCrossingMarker uut;
    uut.setMarkIntraDatasetCrossings(true);
    uut.apply(map);

    CPPUNIT_ASSERT_EQUAL(1, MapUtils::getNumReviews(map));
  }

  void runTagExcludeTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(map, _inputPath + "input.osm", false, Status::Unknown1);
    RailwaysCrossingMarker uut;
    uut.setMarkIntraDatasetCrossings(true);

    // Exclude railway=subway, which one of the features has. No reviews should be generated.

    uut.setTagExcludeFilter(QStringList("railway=subway"));
    uut.apply(map);

    CPPUNIT_ASSERT_EQUAL(0, MapUtils::getNumReviews(map));

    // Exclude railway=tram, which one of the features has. No reviews should be generated.

    uut.setTagExcludeFilter(QStringList("railway=tram"));
    uut.apply(map);

    CPPUNIT_ASSERT_EQUAL(0, MapUtils::getNumReviews(map));
  }

  void runPreventIntraDatasetMarkingTest()
  {
    // Prevent intra-dataset marking. Since all features are read in with the same status, no
    // reviews should be generated.

    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(map, _inputPath + "input.osm", false, Status::Unknown1);

    RailwaysCrossingMarker uut;
    uut.setMarkIntraDatasetCrossings(false);
    uut.apply(map);

    CPPUNIT_ASSERT_EQUAL(0, MapUtils::getNumReviews(map));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(RailwaysCrossingMarkerTest, "quick");

}
