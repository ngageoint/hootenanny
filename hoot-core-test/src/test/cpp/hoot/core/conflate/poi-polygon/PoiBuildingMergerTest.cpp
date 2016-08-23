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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include "../../TestUtils.h"
#include <hoot/core/conflate/poi-polygon/PoiBuildingMerger.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/MapProjector.h>

// Qt
#include <QDir>

namespace hoot
{

class PoiBuildingMergerTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(PoiBuildingMergerTest);
  CPPUNIT_TEST(mergeWayAsBuildingTest);
  CPPUNIT_TEST(mergeRelationAsBuildingTest);
  CPPUNIT_TEST(mergeMissingPoiInputTest);
  CPPUNIT_TEST(mergeMissingBuildingInputTest);
  CPPUNIT_TEST(mergeMoreThanOnePoiInputTest);
  CPPUNIT_TEST(mergeMoreThanOneBuildingInputTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void mergeWayAsBuildingTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, "test-files/conflate/poi-polygon/poi-poly-way-building-in.osm", false,
      Status::Unknown1);

    PoiBuildingMerger::merge(map);

    QDir().mkdir("test-output/conflate/poi-polygon");
    MapProjector::projectToWgs84(map);
    OsmMapWriterFactory::getInstance().write(map,
      "test-output/conflate/poi-polygon/poi-poly-way-building-out.osm");

    HOOT_FILE_EQUALS(
      "test-files/conflate/poi-polygon/poi-poly-way-building-out.osm",
      "test-output/conflate/poi-polygon/poi-poly-way-building-out.osm");
  }

  void mergeRelationAsBuildingTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, "test-files/conflate/poi-polygon/poi-poly-relation-building-in.osm", false,
      Status::Unknown1);

    PoiBuildingMerger::merge(map);

    QDir().mkdir("test-output/conflate/poi-polygon");
    MapProjector::projectToWgs84(map);
    OsmMapWriterFactory::getInstance().write(map,
      "test-output/conflate/poi-polygon/poi-poly-relation-building-out.osm");

    HOOT_FILE_EQUALS(
      "test-files/conflate/poi-polygon/poi-poly-relation-building-out.osm",
      "test-output/conflate/poi-polygon/poi-poly-relation-building-out.osm");
  }

  void mergeMissingPoiInputTest()
  {
    QString exceptionMsg("");
    try
    {
      OsmMapPtr map(new OsmMap());
      OsmMapReaderFactory::read(
        map, "test-files/conflate/poi-polygon/poi-poly-missing-poi-in.osm", false,
        Status::Unknown1);

      PoiBuildingMerger::merge(map);
    }
    catch (HootException e)
    {
      exceptionMsg = e.what();
    }
    HOOT_STR_EQUALS("No POI passed to POI/building merger.", exceptionMsg.toStdString());
  }

  void mergeMissingBuildingInputTest()
  {
    QString exceptionMsg("");
    try
    {
      OsmMapPtr map(new OsmMap());
      OsmMapReaderFactory::read(
        map, "test-files/conflate/poi-polygon/poi-poly-missing-building-in.osm", false,
        Status::Unknown1);

      PoiBuildingMerger::merge(map);
    }
    catch (HootException e)
    {
      exceptionMsg = e.what();
    }
    HOOT_STR_EQUALS("No building passed to POI/building merger.", exceptionMsg.toStdString());
  }

  void mergeMoreThanOnePoiInputTest()
  {
    QString exceptionMsg("");
    try
    {
      OsmMapPtr map(new OsmMap());
      OsmMapReaderFactory::read(
        map, "test-files/conflate/poi-polygon/poi-poly-more-than-one-poi-in.osm", false,
        Status::Unknown1);

      PoiBuildingMerger::merge(map);
    }
    catch (HootException e)
    {
      exceptionMsg = e.what();
    }
    HOOT_STR_EQUALS(
      "More than one POI passed to POI/building merger.", exceptionMsg.toStdString());
  }

  void mergeMoreThanOneBuildingInputTest()
  {
    QString exceptionMsg("");
    try
    {
      OsmMapPtr map(new OsmMap());
      OsmMapReaderFactory::read(
        map, "test-files/conflate/poi-polygon/poi-poly-more-than-one-building-in.osm", false,
        Status::Unknown1);

      PoiBuildingMerger::merge(map);
    }
    catch (HootException e)
    {
      exceptionMsg = e.what();
    }
    HOOT_STR_EQUALS(
      "More than one building passed to POI/building merger.", exceptionMsg.toStdString());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PoiBuildingMergerTest, "quick");

}
