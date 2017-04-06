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

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmJsonWriter.h>
#include <hoot/core/filters/PoiCriterion.h>
#include <hoot/core/filters/HighwayCriterion.h>
#include <hoot/core/visitors/TransliterateNameVisitor.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/io/OsmXmlWriter.h>

// Qt
#include <QDir>

#include "../TestUtils.h"

namespace hoot
{
using namespace Tgs;

class TransliterateNameVisitorTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(TransliterateNameVisitorTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runBasicTest()
  {
    OsmMap::resetCounters();
    boost::shared_ptr<OsmMap> map(new OsmMap());
    NodePtr n1(new Node(Status::Unknown1, map->createNextNodeId(), 0, 0, 10));
    n1->getTags()["name"] = "أَلِف";
    map->addNode(n1);

    TransliterateNameVisitor v;
    map->visitRw(v);
    QStringList names = n1->getTags().getNames();
    CPPUNIT_ASSERT_EQUAL(true, TransliterateNameVisitor::isLatin(names[0]));
    //characters is latin, no "note, Transliterated Name" added.
    CPPUNIT_ASSERT_EQUAL(true, n1->getTags().find("note") == n1->getTags().end());

    OsmMap::resetCounters();
    boost::shared_ptr<OsmMap> map1(new OsmMap());
    NodePtr n2(new Node(Status::Unknown1, map->createNextNodeId(), 500, 500, 10));
    n2->getTags()["name"] = "little town";
    map1->addNode(n2);
    TransliterateNameVisitor v1;
    map1->visitRw(v1);
    QStringList names1 = n2->getTags().getNames();
    CPPUNIT_ASSERT_EQUAL(true, TransliterateNameVisitor::isLatin(names1[0]));
    //characters is latin, no "note, Transliterated Name" added.
    CPPUNIT_ASSERT_EQUAL(true, n2->getTags().find("note") == n2->getTags().end());
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TransliterateNameVisitorTest, "quick");

}


