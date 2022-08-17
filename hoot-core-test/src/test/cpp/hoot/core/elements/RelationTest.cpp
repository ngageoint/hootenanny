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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/io/OsmJsonReader.h>
#include <hoot/core/io/OsmJsonWriter.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/visitors/ElementCountVisitor.h>

//  Geos
#include <geos/geom/Coordinate.h>

using namespace geos::geom;

namespace hoot
{

class RelationTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(RelationTest);
  CPPUNIT_TEST(runCircularVisitRo1Test);
  CPPUNIT_TEST(runCircularVisitRo2Test);
  CPPUNIT_TEST(runCircularVisitRw1Test);
  CPPUNIT_TEST(runCircularVisitRw2Test);
  CPPUNIT_TEST(runCircularEnvelopeTest);
  CPPUNIT_TEST(runReplaceTest1);
  CPPUNIT_TEST(runReplaceTest2);
  CPPUNIT_TEST(runIndexOfTest);
  CPPUNIT_TEST(runInsertTest);
  CPPUNIT_TEST_SUITE_END();

public:

  RelationTest()
    : HootTestFixture("test-files/elements/RelationTest/", "test-output/elements/RelationTest/")
  {
  }

  void runCircularVisitRo1Test()
  {
    DisableLog dl;

    OsmMapPtr map = std::make_shared<OsmMap>();
    RelationPtr r1 = std::make_shared<Relation>(Status::Unknown1, 1, 15);
    RelationPtr r2 = std::make_shared<Relation>(Status::Unknown1, 2, 15);
    map->addElement(r1);
    map->addElement(r2);

    r1->addElement("", r2->getElementId());
    r2->addElement("", r1->getElementId());

    ElementCountVisitor v;
    r1->visitRo(*map, v);

    CPPUNIT_ASSERT_EQUAL(2, v.getCount());
    //  Each have one member even though it is a loop
    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(1), r1->getMemberCount());
    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(1), r2->getMemberCount());
  }

  void runCircularVisitRo2Test()
  {
    DisableLog dl;

    OsmMapPtr map = std::make_shared<OsmMap>();
    RelationPtr r1 = std::make_shared<Relation>(Status::Unknown1, 1, 15);
    RelationPtr r2 = std::make_shared<Relation>(Status::Unknown1, 2, 15);
    map->addElement(r1);
    map->addElement(r2);

    r1->addElement("", r1->getElementId());
    r2->addElement("", r2->getElementId());

    ElementCountVisitor v;
    r1->visitRo(*map, v);
    r2->visitRo(*map, v);

    CPPUNIT_ASSERT_EQUAL(2, v.getCount());
    //  Relations have no members because they are self-references
    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(0), r1->getMemberCount());
    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(0), r2->getMemberCount());
  }

  void runCircularVisitRw1Test()
  {
    DisableLog dl;

    OsmMapPtr map = std::make_shared<OsmMap>();
    RelationPtr r1 = std::make_shared<Relation>(Status::Unknown1, 1, 15);
    RelationPtr r2 = std::make_shared<Relation>(Status::Unknown1, 2, 15);
    map->addElement(r1);
    map->addElement(r2);

    r1->addElement("", r2->getElementId());
    r2->addElement("", r1->getElementId());

    ElementCountVisitor v;
    r1->visitRo(*map, v);

    CPPUNIT_ASSERT_EQUAL(2, v.getCount());
    //  Each have one member even though it is a loop
    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(1), r1->getMemberCount());
    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(1), r2->getMemberCount());
  }

  void runCircularVisitRw2Test()
  {
    DisableLog dl;

    OsmMapPtr map = std::make_shared<OsmMap>();
    RelationPtr r1 = std::make_shared<Relation>(Status::Unknown1, 1, 15);
    RelationPtr r2 = std::make_shared<Relation>(Status::Unknown1, 2, 15);
    map->addElement(r1);
    map->addElement(r2);

    r1->addElement("", r1->getElementId());
    r2->addElement("", r2->getElementId());

    ElementCountVisitor v;
    r1->visitRo(*map, v);
    r2->visitRo(*map, v);

    CPPUNIT_ASSERT_EQUAL(2, v.getCount());
    //  Relations have no members because they are self-references
    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(0), r1->getMemberCount());
    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(0), r2->getMemberCount());
  }

  /**
   * Test to verify that we don't get stuck in an infinite loop with circular references
   */
  void runCircularEnvelopeTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    //  Build two triangle ways spaced one unit apart
    Coordinate c1[] = { Coordinate(0.0, 0.0), Coordinate(1.0, 0.0),
                        Coordinate(1.0, 0.0), Coordinate(0.0, 0.0),
                        Coordinate::getNull() };
    WayPtr way1 = TestUtils::createWay(map, c1, "w1", Status::Unknown1, 5);
    Coordinate c2[] = { Coordinate(2.0, 1.0), Coordinate(3.0, 1.0),
                        Coordinate(3.0, 0.0), Coordinate(2.0, 1.0),
                        Coordinate::getNull() };
    WayPtr way2 = TestUtils::createWay(map, c2, "w2", Status::Unknown1, 5);
    //  Create two relations, each one containing a way and the other relation
    RelationPtr r1 = std::make_shared<Relation>(Status::Unknown1, 1, 15);
    RelationPtr r2 = std::make_shared<Relation>(Status::Unknown1, 2, 15);
    r1->addElement("", way1);
    r1->addElement("", r2);
    r2->addElement("", way2);
    r2->addElement("", r1);
    //  Add the relations to the map
    map->addRelation(r1);
    map->addRelation(r2);
    //  Get the envelopes for both relations
    std::shared_ptr<Envelope> e1(r1->getEnvelope(map));
    std::shared_ptr<Envelope> e2(r2->getEnvelope(map));
    //  Compare the envelopes
    CPPUNIT_ASSERT_EQUAL(e1->getMinX(), e2->getMinX());
    CPPUNIT_ASSERT_EQUAL(e1->getMaxX(), e2->getMaxX());
    CPPUNIT_ASSERT_EQUAL(e1->getMinY(), e2->getMinY());
    CPPUNIT_ASSERT_EQUAL(e1->getMaxY(), e2->getMaxY());
  }

  /**
   * Test to verify we can replace one way in a relation with multiple other ways.
   */
  void runReplaceTest1()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    RelationPtr r1 = std::make_shared<Relation>(Status::Unknown1, 1, 15);
    WayPtr w1 = std::make_shared<Way>(Status::Unknown1, 1, 15);
    WayPtr w2 = std::make_shared<Way>(Status::Unknown1, 2, 15);
    WayPtr w3 = std::make_shared<Way>(Status::Unknown1, 3, 15);
    map->addElement(r1);
    map->addElement(w1);
    map->addElement(w2);
    map->addElement(w3);

    r1->addElement("foo", w1->getElementId());

    QList<ElementPtr> newWays;
    newWays.append(w2);
    newWays.append(w3);

    r1->replaceElement(w1, newWays);

    const QString testFileName = "runReplaceTest1.json";
    OsmJsonWriter writer;
    writer.setIncludeCompatibilityTags(false);
    writer.open(_outputPath + testFileName);
    MapProjector::projectToWgs84(map);
    writer.write(map);
    writer.close();
    HOOT_FILE_EQUALS(_inputPath + testFileName, _outputPath + testFileName);
  }

  /**
   * Test to verify we can replace multiple ways in a relation with multiple other ways and that
   * only the expected way (w1) gets replaced.
   */
  void runReplaceTest2()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    RelationPtr r1 = std::make_shared<Relation>(Status::Unknown1, 1, 15);
    WayPtr w1 = std::make_shared<Way>(Status::Unknown1, 1, 15);
    WayPtr w2 = std::make_shared<Way>(Status::Unknown1, 2, 15);
    WayPtr w3 = std::make_shared<Way>(Status::Unknown1, 3, 15);
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

    const QString testFileName = "runReplaceTest2.json";
    OsmJsonWriter writer;
    writer.setIncludeCompatibilityTags(false);
    writer.open(_outputPath + testFileName);
    MapProjector::projectToWgs84(map);
    writer.write(map);
    writer.close();
    HOOT_FILE_EQUALS(_inputPath + testFileName, _outputPath + testFileName);
  }

  void runIndexOfTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    RelationPtr r1 = std::make_shared<Relation>(Status::Unknown1, 1, 15);
    WayPtr w1 = std::make_shared<Way>(Status::Unknown1, 1, 15);
    WayPtr w2 = std::make_shared<Way>(Status::Unknown1, 2, 15);
    WayPtr w3 = std::make_shared<Way>(Status::Unknown1, 3, 15);
    map->addElement(r1);
    map->addElement(w1);
    map->addElement(w2);
    map->addElement(w3);

    r1->addElement("foo", w1->getElementId());
    r1->addElement("bar", w2->getElementId());
    r1->addElement("lucky", w3->getElementId());

    CPPUNIT_ASSERT_EQUAL((size_t)0, r1->indexOf(w1->getElementId()));
    CPPUNIT_ASSERT_EQUAL((size_t)1, r1->indexOf(w2->getElementId()));
    CPPUNIT_ASSERT_EQUAL((size_t)2, r1->indexOf(w3->getElementId()));
  }

  void runInsertTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    RelationPtr r1 = std::make_shared<Relation>(Status::Unknown1, 1, 15);
    WayPtr w1 = std::make_shared<Way>(Status::Unknown1, 1, 15);
    WayPtr w2 = std::make_shared<Way>(Status::Unknown1, 2, 15);
    map->addElement(r1);
    map->addElement(w1);
    map->addElement(w2);

    r1->addElement("foo", w1->getElementId());
    r1->addElement("bar", w2->getElementId());

    WayPtr w3 = std::make_shared<Way>(Status::Unknown1, 3, 15);
    map->addElement(w3);
    r1->insertElement("lucky", w3->getElementId(), 1);

    CPPUNIT_ASSERT_EQUAL((size_t)0, r1->indexOf(w1->getElementId()));
    CPPUNIT_ASSERT_EQUAL((size_t)1, r1->indexOf(w3->getElementId()));
    CPPUNIT_ASSERT_EQUAL((size_t)2, r1->indexOf(w2->getElementId()));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(RelationTest, "quick");

}


