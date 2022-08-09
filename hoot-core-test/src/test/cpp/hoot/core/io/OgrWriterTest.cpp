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
 * @copyright Copyright (C) 2013, 2014, 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/OgrWriter.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/util/FileUtils.h>

namespace hoot
{

class OgrWriterTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(OgrWriterTest);
  CPPUNIT_TEST(runGdbTest);
  CPPUNIT_TEST(runShpTest);
  CPPUNIT_TEST(runRelationContainingRelationTest);
  CPPUNIT_TEST_SUITE_END();

public:

  OgrWriterTest()
    : HootTestFixture("test-files/io/", "test-output/io/")
  {
  }

  OsmMapPtr _map;

  NodePtr createNode(double x, double y)
  {
    NodePtr n = std::make_shared<Node>(Status::Unknown1, _map->createNextNodeId(), x, y, 10.0);
    _map->addNode(n);
    return n;
  }

  OsmMapPtr createTestMap()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    _map = map;

    NodePtr n1 = createNode(0.0, 0.0);
    n1->setTag(MetadataTags::Building(), "yes");
    n1->setTag("name", "n1");

    WayPtr w1 = std::make_shared<Way>(Status::Unknown1, map->createNextWayId(), 13.0);
    w1->setTag(MetadataTags::Area(), "yes");
    w1->setTag(MetadataTags::Building(), "yes");
    w1->setTag("name", "w1");
    w1->addNode(createNode(0.1, 0.0)->getId());
    w1->addNode(createNode(0.2, 0.0)->getId());
    w1->addNode(createNode(0.2, 0.1)->getId());
    w1->addNode(w1->getNodeId(0));
    map->addWay(w1);

    WayPtr w2 = std::make_shared<Way>(Status::Unknown1, map->createNextWayId(), 13.0);
    w2->setTag("highway", "track");
    w2->setTag("name", "w2");
    w2->addNode(createNode(0.3, 0.0)->getId());
    w2->addNode(createNode(0.3, 0.1)->getId());
    map->addWay(w2);

    WayPtr w3 = std::make_shared<Way>(Status::Unknown1, map->createNextWayId(), 13.0);
    w3->setTag("highway", "road");
    w3->setTag("name", "w3");
    w3->addNode(createNode(0.4, 0.0)->getId());
    w3->addNode(createNode(0.4, 0.1)->getId());
    map->addWay(w3);

    WayPtr w4 = std::make_shared<Way>(Status::Unknown1, map->createNextWayId(), 13.0);
    w4->addNode(createNode(0.5, 0.0)->getId());
    w4->addNode(createNode(0.7, 0.0)->getId());
    w4->addNode(createNode(0.6, 0.1)->getId());
    w4->addNode(w4->getNodeId(0));
    map->addWay(w4);

    WayPtr w5 = std::make_shared<Way>(Status::Unknown1, map->createNextWayId(), 13.0);
    w5->addNode(createNode(0.55, 0.01)->getId());
    w5->addNode(createNode(0.65, 0.01)->getId());
    w5->addNode(createNode(0.6, 0.05)->getId());
    w5->addNode(w5->getNodeId(0));
    map->addWay(w5);

    RelationPtr r1 =
      std::make_shared<Relation>(Status::Unknown1, 1, 15.0, MetadataTags::RelationMultiPolygon());
    r1->setTag(MetadataTags::Building(), "yes");
    r1->setTag("name", "r1");
    r1->addElement(MetadataTags::RoleOuter(), w4->getElementId());
    r1->addElement(MetadataTags::RoleInner(), w5->getElementId());
    map->addRelation(r1);

    return map;
  }

  void runShpTest()
  {
    FileUtils::removeDir(_outputPath + "OgrWriterShpTest");
    OgrWriter uut;
    uut.setSchemaTranslationScript(_inputPath + "SampleTranslation.js");
    uut.open(_outputPath + "OgrWriterShpTest.shp");
    uut.write(createTestMap());
    uut.close();
    QStringList nameFilter;
    nameFilter << "*.shp";

    // Make sure it created the shapefiles, we aren't actually testing for correct output.
    HOOT_STR_EQUALS("[4]{AAL015.shp, LAP010.shp, LAP030.shp, PAL015.shp}",
                    QDir(_outputPath + "OgrWriterShpTest").entryList(nameFilter));
  }

  void runGdbTest()
  {
    FileUtils::removeDir(_outputPath + "OgrWriterTest.gdb");
    OgrWriter uut;
    uut.setSchemaTranslationScript(_inputPath + "SampleTranslation.js");
    uut.open(_outputPath + "OgrWriterTest.gdb");
    uut.write(createTestMap());
    uut.close();

    // make sure it created a bunch of files. We aren't testing for correct output.
    CPPUNIT_ASSERT(QDir(_outputPath + "OgrWriterTest.gdb").entryList().size() > 10);
  }

  // We're testing here that the writer properly writes the relations in two passes (not checking
  // the output, just for an error) and is able to write a relation which references another
  // relation that hasn't been yet written.
  void runRelationContainingRelationTest()
  {
    OsmMapPtr map = createTestMap();

    RelationPtr r2 =
      std::make_shared<Relation>(Status::Unknown1, 2, 15.0, MetadataTags::RelationMultiPolygon());
    r2->setTag(MetadataTags::Building(), "yes");
    r2->setTag("name", "r2");
    r2->addElement(MetadataTags::RoleOuter(), ElementId(ElementType::Way, 1));
    r2->addElement(MetadataTags::RoleInner(), ElementId(ElementType::Way, 2));
    map->addRelation(r2);

    map->getRelation(1)->addElement("test", ElementId(ElementType::Relation, 2));

    FileUtils::removeDir(_outputPath + "OgrWriterRelationTest.gdb");
    OgrWriter uut;
    uut.setSchemaTranslationScript(_inputPath + "SampleTranslation.js");
    uut.open(_outputPath + "OgrWriterRelationTest.gdb");
    uut.write(map);
    uut.close();

    // make sure it created a bunch of files. We aren't testing for correct output.
    CPPUNIT_ASSERT(QDir(_outputPath + "OgrWriterRelationTest.gdb").entryList().size() > 10);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OgrWriterTest, "quick");

}
