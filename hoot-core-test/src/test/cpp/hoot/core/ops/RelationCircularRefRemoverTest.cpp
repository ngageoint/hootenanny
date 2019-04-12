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
 * @copyright Copyright (C) 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/ops/RelationCircularRefRemover.h>
#include <hoot/core/util/Log.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

namespace hoot
{

class RelationCircularRefRemoverTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(RelationCircularRefRemoverTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  RelationCircularRefRemoverTest()
    : HootTestFixture("test-files/ops/RelationCircularRefRemoverTest/",
                      "test-output/ops/RelationCircularRefRemoverTest/")
  {
    setResetType(ResetBasic);
  }

  void runBasicTest()
  {
    OsmXmlReader reader;
    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.setUseDataSourceIds(true);
    reader.read(_inputPath + "RunBasicTestIn.osm", map);

    RelationCircularRefRemover uut;
    uut.apply(map);

    OsmXmlWriter writer;
    writer.setIncludeCompatibilityTags(false);
    writer.write(map, _outputPath + "RunBasicTestOut.osm");

    HOOT_FILE_EQUALS( _inputPath + "RunBasicTestOut.osm",
                     _outputPath + "RunBasicTestOut.osm");
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(RelationCircularRefRemoverTest, "quick");

}
