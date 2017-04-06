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
#include <hoot/core/visitors/TagCountVisitor.h>

// Qt
#include <QDebug>
#include <QDir>

#include "../TestUtils.h"

namespace hoot
{

class TagCountVisitorTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(TagCountVisitorTest);
  CPPUNIT_TEST(totalTagCountTest);
  CPPUNIT_TEST(informationTagCountTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void totalTagCountTest()
  {
    boost::shared_ptr<const OsmMap> constMap = _loadMap();

    TagCountVisitor tagCountVisitor;
    constMap->visitRo(tagCountVisitor);

    CPPUNIT_ASSERT_EQUAL((long)13, (long)tagCountVisitor.getStat());
  }

  void informationTagCountTest()
  {
    boost::shared_ptr<const OsmMap> constMap = _loadMap();

    TagCountVisitor tagCountVisitor;
    constMap->visitRo(tagCountVisitor);

    CPPUNIT_ASSERT_EQUAL((long)7, tagCountVisitor.getInformationCount());
  }

private:

  boost::shared_ptr<const OsmMap> _loadMap()
  {
    OsmXmlReader reader;
    boost::shared_ptr<OsmMap> map(new OsmMap());
    OsmMap::resetCounters();
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/visitors/TagCountVisitorTest.osm", map);
    boost::shared_ptr<const OsmMap> constMap = map;
    return constMap;
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TagCountVisitorTest, "quick");

}


