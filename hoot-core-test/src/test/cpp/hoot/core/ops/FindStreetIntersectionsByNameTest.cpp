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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/ops/FindStreetIntersectionsByName.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

namespace hoot
{

class FindStreetIntersectionsByNameTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(FindStreetIntersectionsByNameTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST(runCaseSensitivityTest);
  CPPUNIT_TEST(runPartialMatchTest);
  CPPUNIT_TEST_SUITE_END();

public:

  FindStreetIntersectionsByNameTest() :
  HootTestFixture(
    "test-files/ops/FindStreetIntersectionsByNameTest/",
    "test-output/ops/FindStreetIntersectionsByNameTest/")
  {
  }

  // TODO: explanations

  void runBasicTest()
  {
    FindStreetIntersectionsByName uut;
    OsmMapPtr map(new OsmMap());
    Settings conf;

    OsmMapReaderFactory::read(map, false, Status::Unknown1, "test-files/DcTigerRoads.osm");

    conf.set("name.criterion.names", "I St NW;19th St NW");
    conf.set("name.criterion.case.sensitive", true);
    conf.set("name.criterion.partial.match", false);
    uut.setConfiguration(conf);

    uut.apply(map);

    OsmMapWriterFactory::write(map, _outputPath + "runBasicTestOut.osm");
    HOOT_FILE_EQUALS(_inputPath + "runBasicTestOut.osm", _outputPath + "runBasicTestOut.osm");
  }

  void runCaseSensitivityTest()
  {
    FindStreetIntersectionsByName uut;
    OsmMapPtr map(new OsmMap());
    OsmMapPtr cachedMap;
    Settings conf;

    {
      OsmMapReaderFactory::read(map, false, Status::Unknown1, "test-files/DcTigerRoads.osm");
      cachedMap.reset(new OsmMap(map));

      conf.set("name.criterion.names", "I ST NW;19th St NW");
      conf.set("name.criterion.case.sensitive", true);
      conf.set("name.criterion.partial.match", false);
      uut.setConfiguration(conf);

      uut.apply(map);

      OsmMapWriterFactory::write(map, _outputPath + "runCaseSensitivity1TestOut.osm");
      HOOT_FILE_EQUALS(
        _inputPath + "runCaseSensitivity1TestOut.osm",
        _outputPath + "runCaseSensitivity1TestOut.osm");
    }

    {
      map.reset(new OsmMap(cachedMap));

      conf.set("name.criterion.names", "I ST NW;19th St NW");
      conf.set("name.criterion.case.sensitive", false);
      conf.set("name.criterion.partial.match", false);
      uut.setConfiguration(conf);

      uut.apply(map);

      OsmMapWriterFactory::write(map, _outputPath + "runCaseSensitivity2TestOut.osm");
      HOOT_FILE_EQUALS(
        _inputPath + "runCaseSensitivity2TestOut.osm",
        _outputPath + "runCaseSensitivity2TestOut.osm");
    }
  }

  void runPartialMatchTest()
  {
    FindStreetIntersectionsByName uut;
    OsmMapPtr map(new OsmMap());
    OsmMapPtr cachedMap;
    Settings conf;

    {
      OsmMapReaderFactory::read(map, false, Status::Unknown1, "test-files/DcTigerRoads.osm");
      cachedMap.reset(new OsmMap(map));

      conf.set("name.criterion.names", "I St;19th St");
      conf.set("name.criterion.case.sensitive", false);
      conf.set("name.criterion.partial.match", false);
      uut.setConfiguration(conf);

      uut.apply(map);

      OsmMapWriterFactory::write(map, _outputPath + "runPartialMatch1TestOut.osm");
      HOOT_FILE_EQUALS(
        _inputPath + "runPartialMatch1TestOut.osm", _outputPath + "runPartialMatch1TestOut.osm");
    }

    {
      map.reset(new OsmMap(cachedMap));

      conf.set("name.criterion.names", "I St;19th St");
      conf.set("name.criterion.case.sensitive", false);
      conf.set("name.criterion.partial.match", true);
      uut.setConfiguration(conf);

      uut.apply(map);

      OsmMapWriterFactory::write(map, _outputPath + "runPartialMatch2TestOut.osm");
      HOOT_FILE_EQUALS(
        _inputPath + "runPartialMatch2TestOut.osm", _outputPath + "runPartialMatch2TestOut.osm");
    }
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(FindStreetIntersectionsByNameTest, "quick");

}
