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
 * @copyright Copyright (C) 2014, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/criterion/PoiCriterion.h>
#include <hoot/core/io/OsmJsonWriter.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/visitors/TransliterateNameVisitor.h>

namespace hoot
{

class TransliterateNameVisitorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(TransliterateNameVisitorTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runBasicTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    NodePtr n1(new Node(Status::Unknown1, map->createNextNodeId(), 0, 0, 10));
    n1->getTags()["name"] = "أَلِف";
    map->addNode(n1);

    TransliterateNameVisitor v;
    map->visitRw(v);
    QStringList names = n1->getTags().getNames();
    CPPUNIT_ASSERT_EQUAL(false, TransliterateNameVisitor::isLatin(names[0]));
    //characters is not latin, "note, Transliterated Name" added.
    CPPUNIT_ASSERT_EQUAL(false, n1->getTags().find("note") == n1->getTags().end());

    OsmMapPtr map1 = std::make_shared<OsmMap>();
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


