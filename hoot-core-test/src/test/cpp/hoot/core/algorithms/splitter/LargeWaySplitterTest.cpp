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
 * @copyright Copyright (C) 2012, 2013, 2014, 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/splitter/LargeWaySplitter.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/elements/MapProjector.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

namespace hoot
{

class LargeWaySplitterTest : public HootTestFixture
{
    CPPUNIT_TEST_SUITE(LargeWaySplitterTest);
    CPPUNIT_TEST(runToyTest);
    CPPUNIT_TEST_SUITE_END();

public:

  LargeWaySplitterTest()
    : HootTestFixture("test-files/algorithms/splitter/",
                      "test-output/algorithms/splitter/")
  {
    setResetType(ResetAll);
  }

  void runToyTest()
  {
    OsmXmlReader reader;
    OsmMapPtr map = std::make_shared<OsmMap>();
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/ToyTestA.osm", map);

    MapProjector::projectToPlanar(map);
    LargeWaySplitter::splitWays(map, 20.0);
    MapProjector::projectToWgs84(map);

    OsmXmlWriter writer;
    writer.write(map, _outputPath + "LargeWaySplitterOutput1.osm");

    HOOT_FILE_EQUALS( _inputPath + "LargeWaySplitterOutput1.osm",
                     _outputPath + "LargeWaySplitterOutput1.osm");
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(LargeWaySplitterTest, "quick");

}
