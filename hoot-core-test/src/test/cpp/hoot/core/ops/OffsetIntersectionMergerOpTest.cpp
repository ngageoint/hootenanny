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

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/ops/OffsetIntersectionMergerOp.h>


using namespace geos::geom;

namespace hoot
{

class OffsetIntersectionMergerOpTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(OffsetIntersectionMergerOpTest);
  CPPUNIT_TEST(runOffsetMergeTest);
  CPPUNIT_TEST_SUITE_END();

public:

  OffsetIntersectionMergerOpTest()
    : HootTestFixture("test-files/ops/OffsetIntersectionMergerOpTest/",
                      "test-output/ops/OffsetIntersectionMergerOpTest/")
  {
    setResetType(ResetBasic);
  }

  void runOffsetMergeTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmXmlReader reader;
    reader.setDefaultStatus(Status::Unknown1);
    reader.setUseDataSourceIds(true);
    reader.read(_inputPath + "OffsetIntersectionMergerOpTestInput.osm", map);

    MapProjector::projectToPlanar(map);

    OffsetIntersectionMergerOp::mergeOffsetIntersections(map);

    MapProjector::projectToWgs84(map);

    OsmXmlWriter writer;
    writer.setIncludeCompatibilityTags(false);
    writer.write(map, _outputPath + "OffsetIntersectionMergerOpTestOutput.osm");

    HOOT_FILE_EQUALS( _inputPath + "OffsetIntersectionMergerOpTestExpected.osm",
                     _outputPath + "OffsetIntersectionMergerOpTestOutput.osm");
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OffsetIntersectionMergerOpTest, "quick");

}

