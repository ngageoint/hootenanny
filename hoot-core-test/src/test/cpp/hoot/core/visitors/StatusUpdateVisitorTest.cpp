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
 * @copyright Copyright (C) 2014, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/visitors/StatusUpdateVisitor.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>

namespace hoot
{

class StatusUpdateVisitorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(StatusUpdateVisitorTest);
  CPPUNIT_TEST(runUpdateTest);
  CPPUNIT_TEST(runUpdateOnlyIfInvalidTest);
  CPPUNIT_TEST_SUITE_END();

public:

  StatusUpdateVisitorTest()
    : HootTestFixture("test-files/visitors/StatusUpdateVisitorTest/",
                      "test-output/visitors/StatusUpdateVisitorTest/")
  {
    setResetType(ResetBasic);
  }

  void runUpdateTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, _inputPath + "StatusUpdateVisitorTest.osm", false, Status::Unknown1);

    StatusUpdateVisitor uut(Status::Unknown2);
    map->visitRw(uut);

    //All statuses in the file should be updated to Unknown2.
    OsmMapWriterFactory::write(map, _outputPath + "RunUpdateTest.osm");

    HOOT_FILE_EQUALS( _inputPath + "RunUpdateTest.osm",
                     _outputPath + "RunUpdateTest.osm");
  }

  void runUpdateOnlyIfInvalidTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, _inputPath + "StatusUpdateVisitorTest.osm", false, Status::Unknown1);

    //set the status on a couple of elements to invalid
    map->getNode(-1)->setStatus(Status::Invalid);
    map->getNode(-1)->getTags()[MetadataTags::HootStatus()] = "Invalid";
    map->getWay(-1)->setStatus(Status::Invalid);
    map->getWay(-1)->getTags()[MetadataTags::HootStatus()] = "Invalid";

    //Only the elements with invalid statuses should be updated with status=Unknown2.
    StatusUpdateVisitor uut(Status::Unknown2, true);
    map->visitRw(uut);

    OsmMapWriterFactory::write(map, _outputPath + "RunUpdateOnlyIfInvalidTest.osm");

    HOOT_FILE_EQUALS( _inputPath + "RunUpdateOnlyIfInvalidTest.osm",
                     _outputPath + "RunUpdateOnlyIfInvalidTest.osm");
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(StatusUpdateVisitorTest, "quick");

}


