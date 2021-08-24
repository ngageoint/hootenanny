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
 * @copyright Copyright (C) 2014, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/visitors/DataSummaryTagVisitor.h>

namespace hoot
{

class DataSummaryTagVisitorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(DataSummaryTagVisitorTest);
  CPPUNIT_TEST(runDataSummaryTest);
  CPPUNIT_TEST_SUITE_END();

public:

  DataSummaryTagVisitorTest()
    : HootTestFixture("test-files/visitors/DataSummaryTagVisitorTest/",
                      "test-output/visitors/DataSummaryTagVisitorTest/")
  {
  }

  void runDataSummaryTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmXmlReader reader;
    reader.setDefaultStatus(Status::Unknown1);
    reader.setUseDataSourceIds(true);
    reader.read(_inputPath + "DataSummaryTagVisitorTestInput.osm", map);

    DataSummaryTagVisitor visitor("a;b;c;d;e;f;g;h;i;j;k;l;m;n;o;p;q;r;s;t;u;v;w;x;y;z");
    map->visitRw(visitor);

    OsmXmlWriter writer;
    writer.setIncludeCompatibilityTags(false);
    writer.write(map, _outputPath + "DataSummaryTagVisitorTestOutput.osm");

    HOOT_FILE_EQUALS( _inputPath + "DataSummaryTagVisitorTestExpected.osm",
                     _outputPath + "DataSummaryTagVisitorTestOutput.osm");
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(DataSummaryTagVisitorTest, "quick");

}


