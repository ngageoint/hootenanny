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
 * @copyright Copyright (C) 2014, 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// geos


// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/linearreference/MultiLineStringLocation.h>
#include <hoot/core/algorithms/linearreference/WayLocation.h>
#include <hoot/core/algorithms/linearreference/WaySublineCollection.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/elements/MapProjector.h>

using namespace geos::geom;

namespace hoot
{

class MultiLineStringLocationTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(MultiLineStringLocationTest);
  CPPUNIT_TEST(runSingleWayTest);
  CPPUNIT_TEST(runMultipleWaysWayLocationOnFirstWayTest);
  CPPUNIT_TEST(runMultipleWaysWayLocationOnLastWayTest);
  CPPUNIT_TEST(runMultipleWaysWayLocationOnMiddleWayTest);
  CPPUNIT_TEST(runWayLocationAtBeginningOfWayTest);
  CPPUNIT_TEST(runWayLocationAtEndOfWayTest);
  CPPUNIT_TEST(runRelationHasNoWaysTest);
  CPPUNIT_TEST(runRelationNotMultiLineStringTest);
  CPPUNIT_TEST(runInvalidWayIndexTest);
  CPPUNIT_TEST(runRelationWayMemberDoesntMatchWayLocationWayTest);
  CPPUNIT_TEST(runRelationContainsFeaturesOtherThanWaysTest);
  CPPUNIT_TEST_SUITE_END();

public:

  MultiLineStringLocationTest()
  {
    setResetType(ResetBasic);
  }

  void runSingleWayTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();

    Coordinate coords[] = { Coordinate(0.0, 0.0), Coordinate(100.0, 0.0),
                            Coordinate(100.0, 10.0), Coordinate(0.0, 10.0),
                            Coordinate::getNull() };
    WayPtr way = TestUtils::createWay(map, coords, "", Status::Unknown1, 1);
    RelationPtr relation =
      std::make_shared<Relation>(
      way->getStatus(), map->createNextRelationId(), way->getCircularError(),
      MetadataTags::RelationMultilineString());
    relation->addElement("", way->getElementId());
    map->addElement(relation);

    MultiLineStringLocation multiLineStringLocation(map, relation, 0,
      WayLocation(map, way, 0, 0.5));

    WayLocation wayLocation = multiLineStringLocation.getWayLocation();
    CPPUNIT_ASSERT_EQUAL(0, wayLocation.getSegmentIndex());
    CPPUNIT_ASSERT_EQUAL(0.5, wayLocation.getSegmentFraction());
    CPPUNIT_ASSERT_EQUAL(Coordinate(50, 0), wayLocation.getCoordinate());
    const ConstWayPtr wayLocationWay = wayLocation.getWay();
    HOOT_STR_EQUALS("Way(-1)", wayLocationWay->getElementId());
    const std::vector<long>& nodeIds = wayLocationWay->getNodeIds();
    CPPUNIT_ASSERT_EQUAL((size_t)4, nodeIds.size());
    CPPUNIT_ASSERT_EQUAL(-1L, nodeIds.at(0));
    CPPUNIT_ASSERT_EQUAL(-2L, nodeIds.at(1));
    CPPUNIT_ASSERT_EQUAL(-3L, nodeIds.at(2));
    CPPUNIT_ASSERT_EQUAL(-4L, nodeIds.at(3));

    WaySublineCollection waySublineCollection = multiLineStringLocation.getWaySublineString();
    CPPUNIT_ASSERT_EQUAL((size_t)1, waySublineCollection.getSublines().size());
    CPPUNIT_ASSERT_EQUAL(50.0, waySublineCollection.getLength());
    WaySubline subline = waySublineCollection.getSublines().at(0);
    HOOT_STR_EQUALS("Way(-1)", subline.getElementId());
    CPPUNIT_ASSERT_EQUAL(WayLocation(map, way, 0, 0.0), subline.getStart());
    CPPUNIT_ASSERT_EQUAL(WayLocation(map, way, 0, 0.5), subline.getEnd());
    CPPUNIT_ASSERT_EQUAL(50.0, subline.getLength());
    HOOT_STR_EQUALS("Way(-1)", subline.getWay()->getElementId());
  }

  void runMultipleWaysWayLocationOnFirstWayTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();

    Coordinate coords1[] = { Coordinate(0.0, 0.0), Coordinate(100.0, 0.0),
                             Coordinate::getNull() };
    Coordinate coords2[] = { Coordinate(100.0, 10.0), Coordinate(0.0, 10.0),
                             Coordinate::getNull() };
    WayPtr way1 = TestUtils::createWay(map, coords1, "", Status::Unknown1, 1);
    WayPtr way2 = TestUtils::createWay(map, coords2, "", Status::Unknown1, 1);
    RelationPtr relation =
      std::make_shared<Relation>(
        way1->getStatus(), map->createNextRelationId(), way1->getCircularError(),
        MetadataTags::RelationMultilineString());
    relation->addElement("", way1->getElementId());
    relation->addElement("", way2->getElementId());
    map->addElement(relation);

    MultiLineStringLocation multiLineStringLocation(map, relation, 0,
      WayLocation(map, way1, 0, 0.5));

    WayLocation wayLocation = multiLineStringLocation.getWayLocation();
    CPPUNIT_ASSERT_EQUAL(0, wayLocation.getSegmentIndex());
    CPPUNIT_ASSERT_EQUAL(0.5, wayLocation.getSegmentFraction());
    CPPUNIT_ASSERT_EQUAL(Coordinate(50, 0), wayLocation.getCoordinate());
    const ConstWayPtr wayLocationWay = wayLocation.getWay();
    HOOT_STR_EQUALS("Way(-1)", wayLocationWay->getElementId());
    const std::vector<long>& nodeIds = wayLocationWay->getNodeIds();
    CPPUNIT_ASSERT_EQUAL((size_t)2, nodeIds.size());
    CPPUNIT_ASSERT_EQUAL(-1L, nodeIds.at(0));
    CPPUNIT_ASSERT_EQUAL(-2L, nodeIds.at(1));

    WaySublineCollection waySublineCollection = multiLineStringLocation.getWaySublineString();
    CPPUNIT_ASSERT_EQUAL((size_t)1, waySublineCollection.getSublines().size());
    CPPUNIT_ASSERT_EQUAL(50.0, waySublineCollection.getLength());
    WaySubline subline = waySublineCollection.getSublines().at(0);
    HOOT_STR_EQUALS("Way(-1)", subline.getElementId());
    CPPUNIT_ASSERT_EQUAL(WayLocation(map, way1, 0, 0.0), subline.getStart());
    CPPUNIT_ASSERT_EQUAL(WayLocation(map, way1, 0, 0.5), subline.getEnd());
    CPPUNIT_ASSERT_EQUAL(50.0, subline.getLength());
    HOOT_STR_EQUALS("Way(-1)", subline.getWay()->getElementId());
  }

  void runMultipleWaysWayLocationOnLastWayTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();

    Coordinate coords1[] = { Coordinate(0.0, 0.0), Coordinate(100.0, 0.0),
                             Coordinate::getNull() };
    Coordinate coords2[] = { Coordinate(100.0, 10.0), Coordinate(0.0, 10.0),
                             Coordinate::getNull() };
    WayPtr way1 = TestUtils::createWay(map, coords1, "", Status::Unknown1, 1);
    WayPtr way2 = TestUtils::createWay(map, coords2, "", Status::Unknown1, 1);
    RelationPtr relation =
      std::make_shared<Relation>(
      way1->getStatus(), map->createNextRelationId(), way1->getCircularError(),
      MetadataTags::RelationMultilineString());
    relation->addElement("", way1->getElementId());
    relation->addElement("", way2->getElementId());
    map->addElement(relation);

    MultiLineStringLocation multiLineStringLocation(map, relation, 1,
      WayLocation(map, way2, 0, 0.5));

    WayLocation wayLocation = multiLineStringLocation.getWayLocation();
    CPPUNIT_ASSERT_EQUAL(0, wayLocation.getSegmentIndex());
    CPPUNIT_ASSERT_EQUAL(0.5, wayLocation.getSegmentFraction());
    CPPUNIT_ASSERT_EQUAL(Coordinate(50, 10), wayLocation.getCoordinate());
    const ConstWayPtr wayLocationWay = wayLocation.getWay();
    HOOT_STR_EQUALS("Way(-2)", wayLocationWay->getElementId());
    const std::vector<long>& nodeIds = wayLocationWay->getNodeIds();
    CPPUNIT_ASSERT_EQUAL((size_t)2, nodeIds.size());
    CPPUNIT_ASSERT_EQUAL(-3L, nodeIds.at(0));
    CPPUNIT_ASSERT_EQUAL(-4L, nodeIds.at(1));

    WaySublineCollection waySublineCollection = multiLineStringLocation.getWaySublineString();
    CPPUNIT_ASSERT_EQUAL((size_t)2, waySublineCollection.getSublines().size());
    CPPUNIT_ASSERT_EQUAL(150.0, waySublineCollection.getLength());
    WaySubline subline = waySublineCollection.getSublines().at(1);
    HOOT_STR_EQUALS("Way(-2)", subline.getElementId());
    CPPUNIT_ASSERT_EQUAL(WayLocation(map, way2, 0, 0.0), subline.getStart());
    CPPUNIT_ASSERT_EQUAL(WayLocation(map, way2, 0, 0.5), subline.getEnd());
    CPPUNIT_ASSERT_EQUAL(50.0, subline.getLength());
    HOOT_STR_EQUALS("Way(-2)", subline.getWay()->getElementId());
  }

  void runMultipleWaysWayLocationOnMiddleWayTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();

    Coordinate coords1[] = { Coordinate(0.0, 0.0), Coordinate(100.0, 0.0),
                             Coordinate::getNull() };
    Coordinate coords2[] = { Coordinate(100.0, 5.0), Coordinate(100.0, 10.0),
                             Coordinate::getNull() };
    Coordinate coords3[] = { Coordinate(100.0, 10.0), Coordinate(0.0, 10.0),
                             Coordinate::getNull() };
    WayPtr way1 = TestUtils::createWay(map, coords1, "", Status::Unknown1, 1);
    WayPtr way2 = TestUtils::createWay(map, coords2, "", Status::Unknown1, 1);
    WayPtr way3 = TestUtils::createWay(map, coords3, "", Status::Unknown1, 1);
    RelationPtr relation =
      std::make_shared<Relation>(
        way1->getStatus(), map->createNextRelationId(), way1->getCircularError(),
        MetadataTags::RelationMultilineString());
    relation->addElement("", way1->getElementId());
    relation->addElement("", way2->getElementId());
    relation->addElement("", way3->getElementId());
    map->addElement(relation);

    MultiLineStringLocation multiLineStringLocation(map, relation, 1,
      WayLocation(map, way2, 0, 0.5));

    WayLocation wayLocation = multiLineStringLocation.getWayLocation();
    CPPUNIT_ASSERT_EQUAL(0, wayLocation.getSegmentIndex());
    CPPUNIT_ASSERT_EQUAL(0.5, wayLocation.getSegmentFraction());
    CPPUNIT_ASSERT_EQUAL(Coordinate(100, 7.5), wayLocation.getCoordinate());
    const ConstWayPtr wayLocationWay = wayLocation.getWay();
    HOOT_STR_EQUALS("Way(-2)", wayLocationWay->getElementId());
    const std::vector<long>& nodeIds = wayLocationWay->getNodeIds();
    CPPUNIT_ASSERT_EQUAL((size_t)2, nodeIds.size());
    CPPUNIT_ASSERT_EQUAL(-3L, nodeIds.at(0));
    CPPUNIT_ASSERT_EQUAL(-4L, nodeIds.at(1));

    WaySublineCollection waySublineCollection = multiLineStringLocation.getWaySublineString();
    CPPUNIT_ASSERT_EQUAL((size_t)2, waySublineCollection.getSublines().size());
    CPPUNIT_ASSERT_EQUAL(102.5, waySublineCollection.getLength());
    WaySubline subline = waySublineCollection.getSublines().at(1);
    HOOT_STR_EQUALS("Way(-2)", subline.getElementId());
    CPPUNIT_ASSERT_EQUAL(WayLocation(map, way2, 0, 0.0), subline.getStart());
    CPPUNIT_ASSERT_EQUAL(WayLocation(map, way2, 0, 0.5), subline.getEnd());
    CPPUNIT_ASSERT_EQUAL(2.5, subline.getLength());
    HOOT_STR_EQUALS("Way(-2)", subline.getWay()->getElementId());
  }

  void runWayLocationAtBeginningOfWayTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();

    Coordinate coords[] = { Coordinate(0.0, 0.0), Coordinate(100.0, 0.0),
                            Coordinate(100.0, 10.0), Coordinate(0.0, 10.0),
                            Coordinate::getNull() };
    WayPtr way = TestUtils::createWay(map, coords, "", Status::Unknown1, 1);
    RelationPtr relation =
      std::make_shared<Relation>(
        way->getStatus(), map->createNextRelationId(), way->getCircularError(),
        MetadataTags::RelationMultilineString());
    relation->addElement("", way->getElementId());
    map->addElement(relation);

    MultiLineStringLocation multiLineStringLocation(map, relation, 0,
      WayLocation(map, way, 0, 0.0));

    WayLocation wayLocation = multiLineStringLocation.getWayLocation();
    CPPUNIT_ASSERT_EQUAL(0, wayLocation.getSegmentIndex());
    CPPUNIT_ASSERT_EQUAL(0.0, wayLocation.getSegmentFraction());
    CPPUNIT_ASSERT_EQUAL(Coordinate(0, 0), wayLocation.getCoordinate());
    const ConstWayPtr wayLocationWay = wayLocation.getWay();
    HOOT_STR_EQUALS("Way(-1)", wayLocationWay->getElementId());
    const std::vector<long>& nodeIds = wayLocationWay->getNodeIds();
    CPPUNIT_ASSERT_EQUAL((size_t)4, nodeIds.size());
    CPPUNIT_ASSERT_EQUAL(-1L, nodeIds.at(0));
    CPPUNIT_ASSERT_EQUAL(-2L, nodeIds.at(1));
    CPPUNIT_ASSERT_EQUAL(-3L, nodeIds.at(2));
    CPPUNIT_ASSERT_EQUAL(-4L, nodeIds.at(3));

    WaySublineCollection waySublineCollection = multiLineStringLocation.getWaySublineString();
    CPPUNIT_ASSERT_EQUAL((size_t)1, waySublineCollection.getSublines().size());
    CPPUNIT_ASSERT_EQUAL(0.0, waySublineCollection.getLength());
    WaySubline subline = waySublineCollection.getSublines().at(0);
    HOOT_STR_EQUALS("Way(-1)", subline.getElementId());
    CPPUNIT_ASSERT_EQUAL(WayLocation(map, way, 0, 0.0), subline.getStart());
    CPPUNIT_ASSERT_EQUAL(WayLocation(map, way, 0, 0.0), subline.getEnd());
    CPPUNIT_ASSERT_EQUAL(0.0, subline.getLength());
    HOOT_STR_EQUALS("Way(-1)", subline.getWay()->getElementId());
  }

  void runWayLocationAtEndOfWayTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();

    Coordinate coords[] = { Coordinate(0.0, 0.0), Coordinate(100.0, 0.0),
                            Coordinate(100.0, 10.0), Coordinate(0.0, 10.0),
                            Coordinate::getNull() };
    WayPtr way = TestUtils::createWay(map, coords, "", Status::Unknown1, 1);
    RelationPtr relation =
      std::make_shared<Relation>(
      way->getStatus(), map->createNextRelationId(), way->getCircularError(),
      MetadataTags::RelationMultilineString());
    relation->addElement("", way->getElementId());
    map->addElement(relation);

    MultiLineStringLocation multiLineStringLocation(map, relation, 0,
      WayLocation(map, way, 0, 1.0));

    WayLocation wayLocation = multiLineStringLocation.getWayLocation();
    CPPUNIT_ASSERT_EQUAL(1, wayLocation.getSegmentIndex());
    CPPUNIT_ASSERT_EQUAL(0.0, wayLocation.getSegmentFraction());
    CPPUNIT_ASSERT_EQUAL(Coordinate(100, 0), wayLocation.getCoordinate());
    const ConstWayPtr wayLocationWay = wayLocation.getWay();
    HOOT_STR_EQUALS("Way(-1)", wayLocationWay->getElementId());
    const std::vector<long>& nodeIds = wayLocationWay->getNodeIds();
    CPPUNIT_ASSERT_EQUAL((size_t)4, nodeIds.size());
    CPPUNIT_ASSERT_EQUAL(-1L, nodeIds.at(0));
    CPPUNIT_ASSERT_EQUAL(-2L, nodeIds.at(1));
    CPPUNIT_ASSERT_EQUAL(-3L, nodeIds.at(2));
    CPPUNIT_ASSERT_EQUAL(-4L, nodeIds.at(3));

    WaySublineCollection waySublineCollection = multiLineStringLocation.getWaySublineString();
    CPPUNIT_ASSERT_EQUAL((size_t)1, waySublineCollection.getSublines().size());
    CPPUNIT_ASSERT_EQUAL(100.0, waySublineCollection.getLength());
    WaySubline subline = waySublineCollection.getSublines().at(0);
    HOOT_STR_EQUALS("Way(-1)", subline.getElementId());
    CPPUNIT_ASSERT_EQUAL(WayLocation(map, way, 0, 0.0), subline.getStart());
    CPPUNIT_ASSERT_EQUAL(WayLocation(map, way, 0, 1.0), subline.getEnd());
    CPPUNIT_ASSERT_EQUAL(100.0, subline.getLength());
    HOOT_STR_EQUALS("Way(-1)", subline.getWay()->getElementId());
  }

  void runRelationHasNoWaysTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();

    Coordinate coords[] = { Coordinate(0.0, 0.0), Coordinate(100.0, 0.0),
                            Coordinate(100.0, 10.0), Coordinate(0.0, 10.0),
                            Coordinate::getNull() };
    WayPtr way = TestUtils::createWay(map, coords, "", Status::Unknown1, 1);
    RelationPtr relation =
      std::make_shared<Relation>(
        way->getStatus(), map->createNextRelationId(), way->getCircularError(),
        MetadataTags::RelationMultilineString());
    map->addElement(relation);

    QString exceptionMsg;
    try
    {
      MultiLineStringLocation multiLineStringLocation(map, relation, 0,
        WayLocation(map, way, 0, 0.5));
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT_EQUAL(
      QString("Feature splitting for multi-line string relations requires that the relation has way members.")
        .toStdString(),
      exceptionMsg.toStdString());
  }

  void runRelationNotMultiLineStringTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();

    Coordinate coords[] = { Coordinate(0.0, 0.0), Coordinate(100.0, 0.0),
                            Coordinate(100.0, 10.0), Coordinate(0.0, 10.0),
                            Coordinate::getNull() };
    WayPtr way = TestUtils::createWay(map, coords, "", Status::Unknown1, 1);
    RelationPtr relation =
      std::make_shared<Relation>(
      way->getStatus(), map->createNextRelationId(), way->getCircularError(),
      MetadataTags::RelationMultiPolygon());
    relation->addElement("", way->getElementId());
    map->addElement(relation);

    QString exceptionMsg;
    try
    {
      MultiLineStringLocation multiLineStringLocation(map, relation, 0,
        WayLocation(map, way, 0, 0.5));
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT_EQUAL(
      QString("Invalid relation type: multipolygon expected multiline string.").toStdString(),
      exceptionMsg.toStdString());
  }

  void runInvalidWayIndexTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();

    Coordinate coords[] = { Coordinate(0.0, 0.0), Coordinate(100.0, 0.0),
                            Coordinate(100.0, 10.0), Coordinate(0.0, 10.0),
                            Coordinate::getNull() };
    WayPtr way = TestUtils::createWay(map, coords, "", Status::Unknown1, 1);
    RelationPtr relation =
      std::make_shared<Relation>(
        way->getStatus(), map->createNextRelationId(), way->getCircularError(),
        MetadataTags::RelationMultilineString());
    relation->addElement("", way->getElementId());
    map->addElement(relation);

    QString exceptionMsg;
    try
    {
      MultiLineStringLocation multiLineStringLocation(map, relation, 1,
        WayLocation(map, way, 0, 0.5));
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT_EQUAL(
      QString("Invalid way index: 1").toStdString(),
      exceptionMsg.toStdString());
  }

  void runRelationWayMemberDoesntMatchWayLocationWayTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();

    Coordinate coords[] = { Coordinate(0.0, 0.0), Coordinate(100.0, 0.0),
                            Coordinate(100.0, 10.0), Coordinate(0.0, 10.0),
                            Coordinate::getNull() };
    WayPtr way = TestUtils::createWay(map, coords, "", Status::Unknown1, 1);
    RelationPtr relation =
      std::make_shared<Relation>(
      way->getStatus(), map->createNextRelationId(), way->getCircularError(),
      MetadataTags::RelationMultilineString());
    relation->addElement("", map->getNode(way->getNodeId(0))->getElementId());
    relation->addElement("", way->getElementId());
    map->addElement(relation);

    QString exceptionMsg;
    try
    {
      MultiLineStringLocation multiLineStringLocation(map, relation, 0,
        WayLocation(map, way, 0, 0.5));
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT_EQUAL(
      QString("Feature at way index: 0 does not match way assigned to way location: Way(-1)")
        .toStdString(),
      exceptionMsg.toStdString());
  }

  void runRelationContainsFeaturesOtherThanWaysTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();

    Coordinate coords1[] = { Coordinate(0.0, 0.0), Coordinate(100.0, 0.0),
                             Coordinate::getNull() };
    Coordinate coords2[] = { Coordinate(100.0, 10.0), Coordinate(0.0, 10.0),
                             Coordinate::getNull() };
    WayPtr way1 = TestUtils::createWay(map, coords1, "", Status::Unknown1, 1);
    WayPtr way2 = TestUtils::createWay(map, coords2, "", Status::Unknown1, 1);
    RelationPtr relation =
      std::make_shared<Relation>(
      way1->getStatus(), map->createNextRelationId(), way1->getCircularError(),
      MetadataTags::RelationMultilineString());
    relation->addElement("", map->getNode(way1->getNodeId(0))->getElementId());
    relation->addElement("", way1->getElementId());
    relation->addElement("", way2->getElementId());
    map->addElement(relation);

    QString exceptionMsg;
    try
    {
      MultiLineStringLocation multiLineStringLocation(map, relation, 1,
        WayLocation(map, way1, 0, 0.5));
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT_EQUAL(
      QString("Feature splitting for multi-line string relations may only occur on relations which contain only ways.")
        .toStdString(),
      exceptionMsg.toStdString());
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MultiLineStringLocationTest, "quick");

}

