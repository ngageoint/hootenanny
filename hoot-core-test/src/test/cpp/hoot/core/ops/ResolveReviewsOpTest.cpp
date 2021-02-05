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
 * @copyright Copyright (C) 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/ops/ResolveReviewsOp.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/elements/MapProjector.h>

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
  CPPUNIT_TEST_SUITE_END();

public:

  const QString inputPath = "test-files/ops/ResolveReviewsOp/";
  const QString outputPath = "test-output/ops/ResolveReviewsOp/";

  ResolveReviewsOpTest()
  {
    setResetType(ResetBasic);
    FileUtils::makeDir(outputPath);
  }

  void runKeepTest()
  {
    OsmXmlReader reader;
    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.setPreserveAllTags(true);
    reader.setUseDataSourceIds(true);
    reader.setUseFileStatus(true);
    reader.read(inputPath + "input.osm", map);

    ResolveReviewsOp uut;
    uut.setResolveType(ResolveReviewsOp::KeepReviews);
    uut.apply(map);

    MapProjector::projectToWgs84(map);
    OsmXmlWriter writer;
    writer.setIncludeIds(true);
    writer.setIncludeHootInfo(true);
    writer.write(map, outputPath + "KeepOutput.osm");
    HOOT_FILE_EQUALS(inputPath + "KeepExpected.osm",
                     outputPath + "KeepOutput.osm");
  }

  void runRemoveTest()
  {
    OsmXmlReader reader;
    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.setPreserveAllTags(true);
    reader.setUseDataSourceIds(true);
    reader.setUseFileStatus(true);
    reader.read(inputPath + "input.osm", map);

    ResolveReviewsOp uut;
    uut.setResolveType(ResolveReviewsOp::RemoveReviews);
    uut.apply(map);

    MapProjector::projectToWgs84(map);
    OsmXmlWriter writer;
    writer.setIncludeIds(true);
    writer.setIncludeHootInfo(true);
    writer.write(map, outputPath + "RemoveOutput.osm");
    HOOT_FILE_EQUALS(inputPath + "RemoveExpected.osm",
                     outputPath + "RemoveOutput.osm");
  }

  void runResolveTest()
  {
    OsmXmlReader reader;
    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.setPreserveAllTags(true);
    reader.setUseDataSourceIds(true);
    reader.setUseFileStatus(true);
    reader.read(inputPath + "input.osm", map);

    ResolveReviewsOp uut;
    uut.setResolveType(ResolveReviewsOp::ResolveReviews);
    uut.apply(map);

    MapProjector::projectToWgs84(map);
    OsmXmlWriter writer;
    writer.setIncludeIds(true);
    writer.setIncludeHootInfo(true);
    writer.write(map, outputPath + "ResolveOutput.osm");
    HOOT_FILE_EQUALS(inputPath + "ResolveExpected.osm",
                     outputPath + "ResolveOutput.osm");
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ResolveReviewsOpTest, "quick");

}
