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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/filters/HighwayCriterion.h>
#include <hoot/rnd/conflate/network/OsmNetworkExtractor.h>

namespace hoot
{

class OsmNetworkExtractorTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(OsmNetworkExtractorTest);
  CPPUNIT_TEST(toyTest);
  CPPUNIT_TEST_SUITE_END();

public:

  /**
   * Extract a toy network and verify that the result is as expected.
   */
  void toyTest()
  {
    OsmMapPtr map(new OsmMap());

    OsmMapReaderFactory::getInstance().read(map, "test-files/conflate/network/ToyInput.osm");

    ElementCriterionPtr c(new HighwayCriterion());
    OsmNetworkExtractor uut;
    uut.setCriterion(c);
    OsmNetworkPtr network = uut.extractNetwork(map);

    HOOT_STR_EQUALS("Node:-169 -- Way:-247 -- Node:-221\n"
      "Node:-175 -- Way:-245 -- Node:-217\n"
      "Node:-165 -- Way:-243 -- Node:-169\n"
      "Node:-229 -- Way:-241 -- Node:-221\n"
      "Node:-171 -- Way:-239 -- Node:-175\n"
      "Node:-221 -- Way:-237 -- Node:-171\n"
      "Node:-169 -- Way:-235 -- Node:-175\n"
      "Node:-171 -- Relation:-477 -- Node:-233",
      network->toString());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OsmNetworkExtractorTest, "quick");

}
