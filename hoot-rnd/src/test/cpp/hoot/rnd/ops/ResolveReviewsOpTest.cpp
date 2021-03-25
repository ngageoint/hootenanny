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
 * @copyright Copyright (C) 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/rnd/ops/ResolveReviewsOp.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

namespace hoot
{

class ResolveReviewsOpTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ResolveReviewsOpTest);
  CPPUNIT_TEST(runKeepTest);
  CPPUNIT_TEST(runRemoveTest);
  CPPUNIT_TEST(runResolveTest);
  CPPUNIT_TEST(runResolveMsTest);
  CPPUNIT_TEST_SUITE_END();

public:

  ResolveReviewsOpTest() :
  HootTestFixture("test-files/rnd/ops/ResolveReviewsOp/", "test-output/rnd/ops/ResolveReviewsOp/")
  {
  }

  void runKeepTest()
  {
    OsmXmlReader reader;
    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.setPreserveAllTags(true);
    reader.setUseDataSourceIds(true);
    reader.setUseFileStatus(true);
    reader.read(_inputPath + "input.osm", map);

    ResolveReviewsOp uut;
    uut.setConfiguration(conf());
    uut.setResolveType(ResolveReviewsOp::KeepReviews);
    uut.apply(map);

    MapProjector::projectToWgs84(map);
    OsmXmlWriter writer;
    writer.setIncludeIds(true);
    writer.setIncludeHootInfo(true);
    writer.write(map, _outputPath + "KeepOutput.osm");
    HOOT_FILE_EQUALS(_inputPath + "KeepExpected.osm",
                     _outputPath + "KeepOutput.osm");
  }

  void runRemoveTest()
  {
    OsmXmlReader reader;
    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.setPreserveAllTags(true);
    reader.setUseDataSourceIds(true);
    reader.setUseFileStatus(true);
    reader.read(_inputPath + "input.osm", map);

    ResolveReviewsOp uut;
    uut.setConfiguration(conf());
    uut.setResolveType(ResolveReviewsOp::RemoveReviews);
    uut.apply(map);

    MapProjector::projectToWgs84(map);
    OsmXmlWriter writer;
    writer.setIncludeIds(true);
    writer.setIncludeHootInfo(true);
    writer.write(map, _outputPath + "RemoveOutput.osm");
    HOOT_FILE_EQUALS(_inputPath + "RemoveExpected.osm",
                     _outputPath + "RemoveOutput.osm");
  }

  void runResolveTest()
  {
    OsmXmlReader reader;
    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.setPreserveAllTags(true);
    reader.setUseDataSourceIds(true);
    reader.setUseFileStatus(true);
    reader.read(_inputPath + "input.osm", map);

    ResolveReviewsOp uut;
    uut.setConfiguration(conf());
    uut.setResolveType(ResolveReviewsOp::ResolveReviews);
    uut.apply(map);

    MapProjector::projectToWgs84(map);
    OsmXmlWriter writer;
    writer.setIncludeIds(true);
    writer.setIncludeHootInfo(true);
    writer.write(map, _outputPath + "ResolveOutput.osm");
    HOOT_FILE_EQUALS(_inputPath + "ResolveExpected.osm",
                     _outputPath + "ResolveOutput.osm");
  }

  void runResolveMsTest()
  {
    // This test is here to illustrate the fact that calling a linear merger may leave
    // multilinestring relations in the output. Generally, this is not desirable and
    // MultilineStringMergeRelationCollapser can be called to remove them.

    OsmXmlReader reader;
    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.setPreserveAllTags(true);
    reader.setUseDataSourceIds(true);
    reader.setUseFileStatus(true);
    reader.read(_inputPath + "runResolveMsTestInput.osm", map);
    // The input test data was already in planar, so make that happen.
    MapProjector::projectToPlanar(map);

    ResolveReviewsOp uut;
    uut.setConfiguration(conf());
    uut.setResolveType(ResolveReviewsOp::ResolveReviews);
    uut.apply(map);

    MapProjector::projectToWgs84(map);
    OsmXmlWriter writer;
    writer.setIncludeIds(true);
    writer.setIncludeHootInfo(true);
    writer.write(map, _outputPath + "runResolveMsTestOutput.osm");
    HOOT_FILE_EQUALS(
      _inputPath + "runResolveMsTestOutput.osm", _outputPath + "runResolveMsTestOutput.osm");
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ResolveReviewsOpTest, "slow");

}
