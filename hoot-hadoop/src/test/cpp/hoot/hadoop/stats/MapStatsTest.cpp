/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// hoot
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/io/OsmPbfReader.h>
#include <hoot/core/TestUtils.h>
#include <hoot/hadoop/stats/MapStats.h>

namespace hoot
{

class MapStatsTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(MapStatsTest);
  CPPUNIT_TEST(testConfiguration);
  CPPUNIT_TEST_SUITE_END();

public:

  void testConfiguration()
  {
    pp::Configuration c;

    MapStats s;
    s.expandEnvelope(Envelope(0, 1, 2, 3));
    s.expandNodeRange(4);
    s.expandNodeRange(5);
    s.expandWayRange(6);
    s.expandWayRange(7);
    s.expandRelationRange(8);
    s.expandRelationRange(9);

    s.write(c);

    MapStats s2;

    s2.read(c);
    // make sure the values are identical
    HOOT_STR_EQUALS(s, s2);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MapStatsTest, "quick");

}

