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

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/visitors/MultilineStringMergeRelationCollapser.h>

namespace hoot
{

class MultilineStringMergeRelationCollapserTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(MultilineStringMergeRelationCollapserTest);
  CPPUNIT_TEST(runBasicTest);
  // TODO: add test for all types
  CPPUNIT_TEST_SUITE_END();

public:

  MultilineStringMergeRelationCollapserTest() :
  HootTestFixture(
    "test-files/visitors/MultilineStringMergeRelationCollapserTest/",
    "test-output/visitors/MultilineStringMergeRelationCollapserTest/")
  {
  }

  void runBasicTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(map, _inputPath + "runBasicTestInput.osm", true, Status::Unknown1);

    MultilineStringMergeRelationCollapser uut;
    uut.setOsmMap(map.get());
    uut.setConfiguration(conf());
    uut.setTypes(QStringList("highway"));
    map->visitRw(uut);

    OsmMapWriterFactory::write(map, _outputPath + "runBasicTestOutput.osm");
    HOOT_FILE_EQUALS(_inputPath + "runBasicTestOutput.osm", _outputPath + "runBasicTestOutput.osm");
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MultilineStringMergeRelationCollapserTest, "quick");

}


