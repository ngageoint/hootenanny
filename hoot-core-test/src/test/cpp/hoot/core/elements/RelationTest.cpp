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
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/io/OsmJsonWriter.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/visitors/ElementCountVisitor.h>
#include <hoot/core/io/OsmJsonReader.h>
#include <hoot/core/elements/MapProjector.h>

namespace hoot
{

class RelationTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(RelationTest);
  CPPUNIT_TEST(runCircularVisitRo1Test);
  CPPUNIT_TEST(runCircularVisitRo2Test);
  CPPUNIT_TEST(runCircularVisitRw1Test);
  CPPUNIT_TEST(runCircularVisitRw2Test);
  CPPUNIT_TEST(runReplaceTest1);
  CPPUNIT_TEST(runReplaceTest2);
  CPPUNIT_TEST(runIndexOfTest);
  CPPUNIT_TEST(runInsertTest);
  CPPUNIT_TEST_SUITE_END();

public:

  RelationTest() :
  HootTestFixture("test-files/elements/RelationTest/", "test-output/elements/RelationTest/")
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

    LOG_VAR(v.getCount());
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

    ElementCountVisitor v;
    r1->visitRo(*map, v);
    r2->visitRo(*map, v);

    LOG_VAR(v.getCount());
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

    LOG_VAR(v.getCount());
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

    ElementCountVisitor v;
    r1->visitRo(*map, v);
    r2->visitRo(*map, v);

    LOG_VAR(v.getCount());
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


