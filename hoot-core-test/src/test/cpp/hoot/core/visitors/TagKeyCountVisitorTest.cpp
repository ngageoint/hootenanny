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
 * @copyright Copyright (C) 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/visitors/TagKeyCountVisitor.h>

#include "../TestUtils.h"

namespace hoot
{

class TagKeyCountVisitorTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(TagKeyCountVisitorTest);
  CPPUNIT_TEST(tagKeyCountTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void tagKeyCountTest()
  {
    shared_ptr<OsmMap> map = _loadMap();

    shared_ptr<TagKeyCountVisitor> visitor(new TagKeyCountVisitor("source"));
    map->visitRo(*visitor);

    CPPUNIT_ASSERT_EQUAL((long)6, (long)visitor->getStat());
  }

private:

  shared_ptr<OsmMap> _loadMap()
  {
    OsmXmlReader reader;
    shared_ptr<OsmMap> map(new OsmMap());
    OsmMap::resetCounters();
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/visitors/TagKeyCountVisitorTest.osm", map);
    return map;
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TagKeyCountVisitorTest, "quick");

}


