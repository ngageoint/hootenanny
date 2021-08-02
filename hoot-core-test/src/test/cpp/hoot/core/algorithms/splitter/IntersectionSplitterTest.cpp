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
 * @copyright Copyright (C) 2012, 2013, 2014, 2015, 2016, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/algorithms/splitter/IntersectionSplitter.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>

#include <hoot/core/TestUtils.h>

namespace hoot
{

class IntersectionSplitterTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(IntersectionSplitterTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST(runTestSimple);
  CPPUNIT_TEST_SUITE_END();

public:

  IntersectionSplitterTest() :
  HootTestFixture(
    "test-files/algorithms/splitter/IntersectionSplitterTest/",
    "test-output/algorithms/splitter/IntersectionSplitterTest/")
  {
    setResetType(ResetBasic);
  }

  void runTest()
  {
    OsmXmlReader reader;
    OsmMapPtr map = std::make_shared<OsmMap>();
    reader.setDefaultStatus(Status::Unknown1);
    reader.read(_inputPath + "IntersectionSplitter.osm", map);

    IntersectionSplitter::splitIntersections(map);

    OsmXmlWriter writer;
    writer.setIncludeCompatibilityTags(false);
    writer.write(map, _outputPath + "IntersectionSplitterTest.osm");

    HOOT_FILE_EQUALS(_inputPath + "IntersectionSplitterOut.osm",
                     _outputPath + "IntersectionSplitterTest.osm");
  }

  void runTestSimple()
  {
    OsmXmlReader reader;
    OsmMapPtr map = std::make_shared<OsmMap>();
    reader.setDefaultStatus(Status::Unknown1);
    reader.read(_inputPath + "SimpleSplitter.osm", map);

    IntersectionSplitter::splitIntersections(map);

    OsmXmlWriter writer;
    writer.setIncludeCompatibilityTags(false);
    writer.write(map, _outputPath + "SimpleSplitterOutput.osm");

    HOOT_FILE_EQUALS(_inputPath + "SimpleSplitterExpected.osm",
                     _outputPath + "SimpleSplitterOutput.osm");
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(IntersectionSplitterTest, "quick");

}
