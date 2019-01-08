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
 * @copyright Copyright (C) 2012, 2013, 2014, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/conflate/Conflator.h>
#include <hoot/core/elements/Element.h>
#include <hoot/core/io/OsmPbfReader.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/util/MapProjector.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

namespace hoot
{

class ConflateTagFilterTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ConflateTagFilterTest);
  //TODO
  //CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  ConflateTagFilterTest()
  {
    TestUtils::mkpath("test-output");
  }

  void runBasicTest()
  {
//    OsmPbfReader reader(true);

//    OsmMapPtr map(new OsmMap());
//    reader.setUseFileStatus(true);
//    reader.read("test-files/ToyTestCombined.pbf", map);

//    Conflator uut;
//    uut.loadSource(map);
//    uut.conflate();

//    OsmMapPtr out(new OsmMap(uut.getBestMap()));
//    MapProjector::projectToWgs84(out);

//    OsmXmlWriter writer;
//    writer.setIncludeIds(true);
//    writer.write(out, "test-output/ConflatorPbfTest.osm");

//    CPPUNIT_ASSERT_EQUAL((size_t)15, out->getWays().size());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ConflateTagFilterTest, "quick");

}
