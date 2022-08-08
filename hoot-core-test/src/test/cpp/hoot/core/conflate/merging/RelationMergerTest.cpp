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
 * @copyright Copyright (C) 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/conflate/merging/RelationMerger.h>

namespace hoot
{

class RelationMergerTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(RelationMergerTest);
  CPPUNIT_TEST(runBasicTest);
  // TODO: add test for empty relation being replaced
  // TODO: add test for relations referencing each other
  CPPUNIT_TEST_SUITE_END();

public:

  RelationMergerTest() :
  HootTestFixture(
    "test-files/conflate/merging/RelationMergerTest/",
    "test-output/conflate/merging/RelationMergerTest/")
  {
    // If uncommenting out the writer include debug tags calls for debugging purposes only for the
    // tests below, then also uncomment this line.
    //setResetType(ResetAll);
  }

  void runBasicTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(map, _inputPath + "runTestInput.osm", true);

    RelationMerger uut;
    uut.setOsmMap(map.get());
    uut.merge(ElementId(ElementType::Relation, 7387470), ElementId(ElementType::Relation, -1));

    MapProjector::projectToWgs84(map);
    //conf().set(ConfigOptions().getWriterIncludeDebugTagsKey(), true);
    OsmMapWriterFactory::write(map, _outputPath + "runTestOutput.osm");

    HOOT_FILE_EQUALS(_inputPath + "runTestOutput.osm", _outputPath + "runTestOutput.osm");
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(RelationMergerTest, "quick");

}
