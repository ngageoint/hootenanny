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
#include <hoot/core/OsmMap.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/io/OsmJsonWriter.h>
#include <hoot/core/visitors/ElementCountVisitor.h>
#include <hoot/core/util/MetadataTags.h>

#include "../TestUtils.h"

namespace hoot
{

class RelationTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(RelationTest);
  CPPUNIT_TEST(runCircularVisitRo1Test);
  CPPUNIT_TEST(runCircularVisitRo2Test);
  CPPUNIT_TEST(runCircularVisitRw1Test);
  CPPUNIT_TEST(runCircularVisitRw2Test);
  CPPUNIT_TEST(runReplaceTest1);
  CPPUNIT_TEST(runReplaceTest2);
  CPPUNIT_TEST_SUITE_END();

public:

  void runCircularVisitRo1Test()
  {
    DisableLog dl;

    boost::shared_ptr<OsmMap> map(new OsmMap());
    boost::shared_ptr<Relation> r1(new Relation(Status::Unknown1, 1, 15));
    boost::shared_ptr<Relation> r2(new Relation(Status::Unknown1, 2, 15));
    map->addElement(r1);
    map->addElement(r2);

    r1->addElement("", r2->getElementId());
    r2->addElement("", r1->getElementId());

    ElementCountVisitor v;
    r1->visitRo(*map, v);

    LOG_VAR(v.getCount());
  }

  void runCircularVisitRo2Test()
  {
    DisableLog dl;

    boost::shared_ptr<OsmMap> map(new OsmMap());
    boost::shared_ptr<Relation> r1(new Relation(Status::Unknown1, 1, 15));
    boost::shared_ptr<Relation> r2(new Relation(Status::Unknown1, 2, 15));
    map->addElement(r1);
    map->addElement(r2);

    r1->addElement("", r1->getElementId());

    ElementCountVisitor v;
    r1->visitRo(*map, v);
    r2->visitRo(*map, v);

    LOG_VAR(v.getCount());
  }

  void runCircularVisitRw1Test()
  {
    DisableLog dl;

    boost::shared_ptr<OsmMap> map(new OsmMap());
    boost::shared_ptr<Relation> r1(new Relation(Status::Unknown1, 1, 15));
    boost::shared_ptr<Relation> r2(new Relation(Status::Unknown1, 2, 15));
    map->addElement(r1);
    map->addElement(r2);

    r1->addElement("", r2->getElementId());
    r2->addElement("", r1->getElementId());

    ElementCountVisitor v;
    r1->visitRw(*map, v);

    LOG_VAR(v.getCount());
  }

  void runCircularVisitRw2Test()
  {
    DisableLog dl;

    boost::shared_ptr<OsmMap> map(new OsmMap());
    boost::shared_ptr<Relation> r1(new Relation(Status::Unknown1, 1, 15));
    boost::shared_ptr<Relation> r2(new Relation(Status::Unknown1, 2, 15));
    map->addElement(r1);
    map->addElement(r2);

    r1->addElement("", r1->getElementId());

    ElementCountVisitor v;
    r1->visitRw(*map, v);
    r2->visitRw(*map, v);

    LOG_VAR(v.getCount());
  }

  /**
   * Test to verify we can replace one way in a relation with multiple other ways.
   */
  void runReplaceTest1()
  {
    boost::shared_ptr<OsmMap> map(new OsmMap());
    RelationPtr r1(new Relation(Status::Unknown1, 1, 15));
    WayPtr w1(new Way(Status::Unknown1, 1, 15));
    WayPtr w2(new Way(Status::Unknown1, 2, 15));
    WayPtr w3(new Way(Status::Unknown1, 3, 15));
    map->addElement(r1);
    map->addElement(w1);
    map->addElement(w2);
    map->addElement(w3);

    r1->addElement("foo", w1->getElementId());

    QList<ElementPtr> newWays;
    newWays.append(w2);
    newWays.append(w3);

    r1->replaceElement(w1, newWays);

    HOOT_STR_EQUALS("{\"version\": 0.6,\"generator\": \"Hootenanny\",\"elements\": [\n"
      "{\"type\":\"way\",\"id\":1,\"nodes\":[],\"tags\":{\"" + MetadataTags::ErrorCircular() + "\":\"15\"},\n"
      "{\"type\":\"way\",\"id\":2,\"nodes\":[],\"tags\":{\"" + MetadataTags::ErrorCircular() + "\":\"15\"},\n"
      "{\"type\":\"way\",\"id\":3,\"nodes\":[],\"tags\":{\"" + MetadataTags::ErrorCircular() + "\":\"15\"},\n"
      "{\"type\":\"relation\",\"id\":1,\"members\":[\n"
      "{\"type\":\"way\",\"ref\":2,\"role\":\"foo\"},\n"
      "{\"type\":\"way\",\"ref\":3,\"role\":\"foo\"}],\"tags\":{\"" + MetadataTags::ErrorCircular() + "\":\"15\"}]\n"
      "}\n",
      OsmJsonWriter().toString(map));
  }

  /**
   * Test to verify we can replace multiple ways in a relation with multiple other ways and that
   * only the expected way (w1) gets replaced.
   */
  void runReplaceTest2()
  {
    boost::shared_ptr<OsmMap> map(new OsmMap());
    RelationPtr r1(new Relation(Status::Unknown1, 1, 15));
    WayPtr w1(new Way(Status::Unknown1, 1, 15));
    WayPtr w2(new Way(Status::Unknown1, 2, 15));
    WayPtr w3(new Way(Status::Unknown1, 3, 15));
    map->addElement(r1);
    map->addElement(w1);
    map->addElement(w2);
    map->addElement(w3);

    r1->addElement("foo", w2->getElementId());
    r1->addElement("bar", w1->getElementId());
    r1->addElement("lucky", w1->getElementId());

    QList<ElementPtr> newWays;
    newWays.append(w2);
    newWays.append(w3);

    r1->replaceElement(w1, newWays);

    HOOT_STR_EQUALS("{\"version\": 0.6,\"generator\": \"Hootenanny\",\"elements\": [\n"
      "{\"type\":\"way\",\"id\":1,\"nodes\":[],\"tags\":{\"" + MetadataTags::ErrorCircular() + "\":\"15\"},\n"
      "{\"type\":\"way\",\"id\":2,\"nodes\":[],\"tags\":{\"" + MetadataTags::ErrorCircular() + "\":\"15\"},\n"
      "{\"type\":\"way\",\"id\":3,\"nodes\":[],\"tags\":{\"" + MetadataTags::ErrorCircular() + "\":\"15\"},\n"
      "{\"type\":\"relation\",\"id\":1,\"members\":[\n"
      "{\"type\":\"way\",\"ref\":2,\"role\":\"foo\"},\n"
      "{\"type\":\"way\",\"ref\":2,\"role\":\"bar\"},\n"
      "{\"type\":\"way\",\"ref\":3,\"role\":\"bar\"},\n"
      "{\"type\":\"way\",\"ref\":2,\"role\":\"lucky\"},\n"
      "{\"type\":\"way\",\"ref\":3,\"role\":\"lucky\"}],\"tags\":{\"" + MetadataTags::ErrorCircular() + "\":\"15\"}]\n"
      "}\n",
      OsmJsonWriter().toString(map));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(RelationTest, "quick");

}


