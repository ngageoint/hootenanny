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
 * @copyright Copyright (C) 2014, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/visitors/StatusUpdateVisitor.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>

#include "../TestUtils.h"

// Qt
#include <QDir>

namespace hoot
{

class StatusUpdateVisitorTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(StatusUpdateVisitorTest);
  CPPUNIT_TEST(runUpdateTest);
  CPPUNIT_TEST(runUpdateOnlyIfInvalidTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void setUp()
  {
    TestUtils::mkpath("test-output/visitors");
  }

  void runUpdateTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMap::resetCounters();
    OsmMapReaderFactory::read(
      map, "test-files/visitors/StatusUpdateVisitorTest.osm", false, Status::Unknown1);

    StatusUpdateVisitor uut(Status::Unknown2);
    map->visitRw(uut);

    //All statuses in the file should be updated to Unknown2.
    OsmMapWriterFactory::getInstance().write(map,
      "test-output/visitors/StatusUpdateVisitorTest-runUpdateTest.osm");

    HOOT_FILE_EQUALS(
      "test-files/visitors/StatusUpdateVisitorTest-runUpdateTest.osm",
      "test-output/visitors/StatusUpdateVisitorTest-runUpdateTest.osm");
  }

  void runUpdateOnlyIfInvalidTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMap::resetCounters();
    OsmMapReaderFactory::read(
      map, "test-files/visitors/StatusUpdateVisitorTest.osm", false, Status::Unknown1);

    //set the status on a couple of elements to invalid
    map->getNode(-1)->setStatus(Status::Invalid);
    map->getNode(-1)->getTags()[MetadataTags::HootStatus()] = "Invalid";
    map->getWay(-1)->setStatus(Status::Invalid);
    map->getWay(-1)->getTags()[MetadataTags::HootStatus()] = "Invalid";

    //Only the elements with invalid statuses should be updated with status=Unknown2.
    StatusUpdateVisitor uut(Status::Unknown2, true);
    map->visitRw(uut);

    OsmMapWriterFactory::getInstance().write(map,
      "test-output/visitors/StatusUpdateVisitorTest-runUpdateOnlyIfInvalidTest.osm");

    HOOT_FILE_EQUALS(
      "test-files/visitors/StatusUpdateVisitorTest-runUpdateOnlyIfInvalidTest.osm",
      "test-output/visitors/StatusUpdateVisitorTest-runUpdateOnlyIfInvalidTest.osm");
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(StatusUpdateVisitorTest, "quick");

}


