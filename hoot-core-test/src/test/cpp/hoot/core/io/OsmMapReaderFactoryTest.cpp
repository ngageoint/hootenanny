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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/util/ConfigOptions.h>

namespace hoot
{

class OsmMapReaderFactoryTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(OsmMapReaderFactoryTest);
  CPPUNIT_TEST(runUnsupportedBoundingBoxRead);
  CPPUNIT_TEST_SUITE_END();

public:

  OsmMapReaderFactoryTest() : HootTestFixture("test-files/", UNUSED_PATH)
  {
    setResetType(ResetAll);
  }

  void runUnsupportedBoundingBoxRead()
  {
    conf().set(ConfigOptions::getBoundsKey(), "-180,-90,180,90");
    // map and data inputs don't matter; we just want to see that it throws for any reader other
    // than db readers when the bounds option is used (only readers that currently support it)
    OsmMapPtr map(new OsmMap());

    QString exceptionMsg("");
    try
    {
      OsmMapReaderFactory::read(
        map, _inputPath + "DcGisRoads.pbf", false, Status::Unknown1);
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(
      exceptionMsg.contains(
        ConfigOptions::getBoundsKey() +
        " configuration option used with unsupported reader"));

  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OsmMapReaderFactoryTest, "quick");

}
