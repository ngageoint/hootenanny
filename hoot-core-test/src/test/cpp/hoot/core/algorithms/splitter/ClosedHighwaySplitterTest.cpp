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
 * @copyright Copyright (C) 2023 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/splitter/ClosedHighwaySplitter.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/util/ConfigOptions.h>

namespace hoot
{

class ClosedHighwaySplitterTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ClosedHighwaySplitterTest);
  CPPUNIT_TEST(runClosedHighwayTest);
  CPPUNIT_TEST_SUITE_END();

public:

  ClosedHighwaySplitterTest()
    : HootTestFixture("test-files/algorithms/splitter/",
                      "test-output/algorithms/splitter/")
  {
  }

  void runClosedHighwayTest()
  {
    OsmXmlReader reader;

    OsmMapPtr map = std::make_shared<OsmMap>();
    reader.setDefaultStatus(Status::Unknown1);
    reader.read(_inputPath + "ClosedHighwaySplitter.osm", map);

    MapProjector::projectToPlanar(map);

    ClosedHighwaySplitter::splitClosedHighways(map);

    MapProjector::projectToWgs84(map);

    OsmXmlWriter writer;
    writer.setIncludeCompatibilityTags(false);
    writer.write(map, _outputPath + "ClosedHighwaySplitterOut.osm");

    HOOT_FILE_EQUALS( _inputPath + "ClosedHighwaySplitterExpected.osm",
                     _outputPath + "ClosedHighwaySplitterOut.osm");
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ClosedHighwaySplitterTest, "quick");

}
