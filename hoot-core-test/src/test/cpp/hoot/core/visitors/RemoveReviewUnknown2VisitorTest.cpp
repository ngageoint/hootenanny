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
 * @copyright Copyright (C) 2019, 2021 Maxar (http://www.maxar.com/)
 */

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/visitors/RemoveReviewUnknown2Visitor.h>

namespace hoot
{

class RemoveReviewUnknown2VisitorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(RemoveReviewUnknown2VisitorTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST_SUITE_END();

public:

  RemoveReviewUnknown2VisitorTest()
    : HootTestFixture("test-files/visitors/RemoveReviewUnknown2VisitorTest/",
                      "test-output/visitors/RemoveReviewUnknown2VisitorTest/")
  {
  }

  void runTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmXmlReader reader;
    reader.setUseDataSourceIds(true);
    reader.setKeepStatusTag(true);
    reader.setUseFileStatus(true);
    reader.read(_inputPath + "RemoveReviewUnknown2VisitorInput.osm", map);

    RemoveReviewUnknown2Visitor removeReviews;
    removeReviews.setOsmMap(map.get());
    map->visitRw(removeReviews);

    OsmXmlWriter writer;
    writer.write(map, _outputPath + "RemoveReviewUnknown2VisitorTest.osm");
    HOOT_FILE_EQUALS(_inputPath + "RemoveReviewUnknown2VisitorTest.osm",
                     _outputPath + "RemoveReviewUnknown2VisitorTest.osm");
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(RemoveReviewUnknown2VisitorTest, "quick");

}
