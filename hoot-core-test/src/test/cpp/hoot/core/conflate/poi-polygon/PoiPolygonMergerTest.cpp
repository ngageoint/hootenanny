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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include "../../TestUtils.h"
#include <hoot/core/elements/ElementVisitor.h>
#include <hoot/core/io/OsmJsonWriter.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/conflate/poi-polygon/PoiPolygonMerger.h>
#include <hoot/core/MapProjector.h>

// Qt
#include <QDir>

namespace hoot
{

/**
 * See "Hootenanny - POI to Building" power point for a description of the tests.
 */
class PoiPolygonMergerTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(PoiPolygonMergerTest);
  CPPUNIT_TEST(basicTest);
  CPPUNIT_TEST(toyScenario1Test);
  CPPUNIT_TEST(toyScenario2Test);
  CPPUNIT_TEST(toyScenario3Test);
  CPPUNIT_TEST(toyScenario4Test);
  CPPUNIT_TEST(toyScenario5Test);
  CPPUNIT_TEST(toyScenario6Test);
  CPPUNIT_TEST(mergeWayAsPolyTest);
  CPPUNIT_TEST(mergeRelationAsPolyTest);
  CPPUNIT_TEST(mergeMissingPoiInputTest);
  CPPUNIT_TEST(mergeMissingPolyInputTest);
  CPPUNIT_TEST(mergeMoreThanOnePoiInputTest);
  CPPUNIT_TEST(mergeMoreThanOnePolyInputTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void basicTest()
  {
    OsmMap::resetCounters();
    OsmMapPtr map(new OsmMap());

    Coordinate c1[] = { Coordinate(0.0, 0.0), Coordinate(20.0, 0.0),
                        Coordinate(20.0, 20.0), Coordinate(0.0, 20.0),
                        Coordinate(0.0, 0.0),
                        Coordinate::getNull() };
    WayPtr w1 = TestUtils::createWay(map, Status::Unknown1, c1, 5, "w1");
    w1->getTags().set("area", true);
    w1->getTags()["name"] = "foo";
    w1->getTags()["amenity"] = "bar";
    NodePtr n1(new Node(Status::Unknown2, 1, 10, 10, 5));
    n1->getTags()["name"] = "bar";
    n1->getTags()["amenity"] = "cafe";
    map->addNode(n1);

    {
      OsmMapPtr map2(new OsmMap(map));
      set< pair<ElementId, ElementId> > s;
      s.insert(pair<ElementId, ElementId>(w1->getElementId(), n1->getElementId()));
      PoiPolygonMerger uut(s);
      vector< pair<ElementId, ElementId> > replaced;
      uut.apply(map2, replaced);

      HOOT_STR_EQUALS("{\"version\": 0.6,\"generator\": \"Hootenanny\",\"elements\": [\n"
                      "{\"type\":\"node\",\"id\":-1,\"lat\":0,\"lon\":0},\n"
                      "{\"type\":\"node\",\"id\":-2,\"lat\":0,\"lon\":20},\n"
                      "{\"type\":\"node\",\"id\":-3,\"lat\":20,\"lon\":20},\n"
                      "{\"type\":\"node\",\"id\":-4,\"lat\":20,\"lon\":0},\n"
                      "{\"type\":\"node\",\"id\":-5,\"lat\":0,\"lon\":0},\n"
                      "{\"type\":\"way\",\"id\":-1,\"nodes\":[-1,-2,-3,-4,-5],\"tags\":{\"note\":\"w1\",\"alt_name\":\"bar\",\"name\":\"foo\",\"area\":\"yes\",\"amenity\":\"bar\",\"error:circular\":\"5\"}]\n"
                      "}\n"
                      "", OsmJsonWriter().toString(map2));
      //LOG_VAR(TestUtils::toQuotedString(OsmJsonWriter().toString(map)));
    }

    {
      w1->setStatus(Status::Unknown2);
      n1->setStatus(Status::Unknown1);
      OsmMapPtr map2(new OsmMap(map));
      set< pair<ElementId, ElementId> > s;
      s.insert(pair<ElementId, ElementId>(w1->getElementId(), n1->getElementId()));
      PoiPolygonMerger uut(s);
      vector< pair<ElementId, ElementId> > replaced;
      uut.apply(map2, replaced);

      HOOT_STR_EQUALS("{\"version\": 0.6,\"generator\": \"Hootenanny\",\"elements\": [\n"
                      "{\"type\":\"node\",\"id\":-1,\"lat\":0,\"lon\":0},\n"
                      "{\"type\":\"node\",\"id\":-2,\"lat\":0,\"lon\":20},\n"
                      "{\"type\":\"node\",\"id\":-3,\"lat\":20,\"lon\":20},\n"
                      "{\"type\":\"node\",\"id\":-4,\"lat\":20,\"lon\":0},\n"
                      "{\"type\":\"node\",\"id\":-5,\"lat\":0,\"lon\":0},\n"
                      "{\"type\":\"way\",\"id\":-1,\"nodes\":[-1,-2,-3,-4,-5],\"tags\":{\"note\":\"w1\",\"alt_name\":\"foo\",\"name\":\"bar\",\"area\":\"yes\",\"amenity\":\"cafe\",\"error:circular\":\"5\"}]\n"
                      "}\n"
                      "", OsmJsonWriter().toString(map2));
    }
  }

  class AddPairsVisitor : public ElementVisitor
  {
  public:
    AddPairsVisitor(OsmMapPtr map, QString scenario) : _map(map), _scenario(scenario) {}

    set< pair<ElementId, ElementId> > getPairs() const
    {
      set< pair<ElementId, ElementId> > result;

      for (size_t i = 0; i < max(_e1.size(), _e2.size()); i++)
      {
        size_t i1 = min(i, _e1.size() - 1);
        size_t i2 = min(i, _e2.size() - 1);
        result.insert(pair<ElementId, ElementId>(_e1[i1], _e2[i2]));
      }

      return result;
    }

    virtual void visit(const ConstElementPtr& e)
    {
      ElementId eid = e->getElementId();

      ElementPtr ee = _map->getElement(eid);

      if (ee->getTags().get("note") == _scenario)
      {
        if (ee->getStatus() == Status::Unknown1)
        {
          _e1.push_back(eid);
        }
        else if (ee->getStatus() == Status::Unknown2)
        {
          _e2.push_back(eid);
        }
      }
      else
      {
        RecursiveElementRemover(eid).apply(_map);
      }
    }

  private:
    set< pair<ElementId, ElementId> > _pairs;
    vector<ElementId> _e1, _e2;
    OsmMapPtr _map;
    QString _scenario;
  };

  set< pair<ElementId, ElementId> > _addPairs(QString scenario, OsmMapPtr map)
  {
    AddPairsVisitor v(map, scenario);
    map->visitRw(v);
    return v.getPairs();
  }

  void toyScenario1Test()
  {
    OsmMap::resetCounters();
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::getInstance().read(map, "test-files/conflate/PoiBuildingA.osm", false,
                                            Status::Unknown1);
    OsmMapReaderFactory::getInstance().read(map, "test-files/conflate/PoiBuildingB.osm", false,
                                            Status::Unknown2);

    set< pair<ElementId, ElementId> > s = _addPairs("Toy Scenario 1", map);

    PoiPolygonMerger uut(s);
    vector< pair<ElementId, ElementId> > replaced;
    uut.apply(map, replaced);

    HOOT_STR_EQUALS("[6]{(Way:-22, Relation:-1), (Way:-21, Relation:-1), (Way:-20, Relation:-1), (Way:-19, Relation:-1), (Way:-6, Relation:-1), (Node:-145, Relation:-1)}",
                    replaced);
    HOOT_STR_EQUALS("{\"version\": 0.6,\"generator\": \"Hootenanny\",\"elements\": [\n"
      "{\"type\":\"node\",\"id\":-129,\"lat\":0.00173,\"lon\":-0.004071},\n"
      "{\"type\":\"node\",\"id\":-130,\"lat\":0.001743,\"lon\":-0.003632},\n"
      "{\"type\":\"node\",\"id\":-131,\"lat\":0.00247,\"lon\":-0.003632},\n"
      "{\"type\":\"node\",\"id\":-132,\"lat\":0.002458,\"lon\":-0.004083},\n"
      "{\"type\":\"node\",\"id\":-133,\"lat\":0.003085,\"lon\":-0.002704},\n"
      "{\"type\":\"node\",\"id\":-134,\"lat\":0.003085,\"lon\":-0.003531},\n"
      "{\"type\":\"node\",\"id\":-135,\"lat\":0.002708,\"lon\":-0.003531},\n"
      "{\"type\":\"node\",\"id\":-136,\"lat\":0.002708,\"lon\":-0.002704},\n"
      "{\"type\":\"node\",\"id\":-137,\"lat\":0.00178,\"lon\":-0.002515},\n"
      "{\"type\":\"node\",\"id\":-138,\"lat\":0.001755,\"lon\":-0.002039},\n"
      "{\"type\":\"node\",\"id\":-139,\"lat\":0.002533,\"lon\":-0.002026},\n"
      "{\"type\":\"node\",\"id\":-140,\"lat\":0.002533,\"lon\":-0.002503},\n"
      "{\"type\":\"node\",\"id\":-141,\"lat\":0.001241,\"lon\":-0.003469},\n"
      "{\"type\":\"node\",\"id\":-142,\"lat\":0.001229,\"lon\":-0.002653},\n"
      "{\"type\":\"node\",\"id\":-143,\"lat\":0.001617,\"lon\":-0.002666},\n"
      "{\"type\":\"node\",\"id\":-144,\"lat\":0.001617,\"lon\":-0.003443},\n"
      "{\"type\":\"way\",\"id\":-22,\"nodes\":[-144,-143,-142,-141,-144],\"tags\":{\"building:part\":\"yes\",\"error:circular\":\"15\"},\n"
      "{\"type\":\"way\",\"id\":-21,\"nodes\":[-140,-139,-138,-137,-140],\"tags\":{\"building:part\":\"yes\",\"error:circular\":\"15\"},\n"
      "{\"type\":\"way\",\"id\":-20,\"nodes\":[-136,-135,-134,-133,-136],\"tags\":{\"building:part\":\"yes\",\"error:circular\":\"15\"},\n"
      "{\"type\":\"way\",\"id\":-19,\"nodes\":[-132,-131,-130,-129,-132],\"tags\":{\"building:part\":\"yes\",\"error:circular\":\"15\"},\n"
      "{\"type\":\"relation\",\"id\":-1,\"members\":[\n"
      "{\"type\":\"way\",\"ref\":-22,\"role\":\"part\"},\n"
      "{\"type\":\"way\",\"ref\":-21,\"role\":\"part\"},\n"
      "{\"type\":\"way\",\"ref\":-20,\"role\":\"part\"},\n"
      "{\"type\":\"way\",\"ref\":-19,\"role\":\"part\"}],\"tags\":{\"note\":\"Toy Scenario 1\",\"poi\":\"yes\",\"alt_name\":\"B;C;D;E;F\",\"building\":\"yes\",\"name\":\"A\",\"error:circular\":\"15\"}]\n"
      "}\n",
      OsmJsonWriter(4).toString(map));
  }

  void toyScenario2Test()
  {
    OsmMap::resetCounters();
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::getInstance().read(map, "test-files/conflate/PoiBuildingA.osm", false,
                                            Status::Unknown1);
    OsmMapReaderFactory::getInstance().read(map, "test-files/conflate/PoiBuildingB.osm", false,
                                            Status::Unknown2);

    set< pair<ElementId, ElementId> > s = _addPairs("Toy Scenario 2", map);

    PoiPolygonMerger uut(s);
    vector< pair<ElementId, ElementId> > replaced;
    uut.apply(map, replaced);

    //OsmMapWriterFactory::getInstance().write(map, "test-output/conflate/PoiBuildingToy2.osm");
    HOOT_STR_EQUALS("[6]{(Way:-18, Relation:-1), (Way:-17, Relation:-1), (Way:-16, Relation:-1), (Way:-15, Relation:-1), (Way:-5, Relation:-1), (Node:-120, Relation:-1)}",
                    replaced);
    HOOT_STR_EQUALS("{\"version\": 0.6,\"generator\": \"Hootenanny\",\"elements\": [\n"
                    "{\"type\":\"node\",\"id\":-112,\"lat\":0.002596,\"lon\":0.0009711},\n"
                    "{\"type\":\"node\",\"id\":-113,\"lat\":0.002771,\"lon\":-0.0005339},\n"
                    "{\"type\":\"node\",\"id\":-114,\"lat\":0.001818,\"lon\":0.0009586},\n"
                    "{\"type\":\"node\",\"id\":-115,\"lat\":0.00168,\"lon\":0.0003315},\n"
                    "{\"type\":\"node\",\"id\":-116,\"lat\":0.00168,\"lon\":-0.0004461},\n"
                    "{\"type\":\"node\",\"id\":-117,\"lat\":0.001291,\"lon\":0.000344},\n"
                    "{\"type\":\"node\",\"id\":-118,\"lat\":0.00252,\"lon\":-0.001086},\n"
                    "{\"type\":\"node\",\"id\":-119,\"lat\":0.002596,\"lon\":0.0004945},\n"
                    "{\"type\":\"node\",\"id\":-121,\"lat\":0.003147,\"lon\":-0.0005339},\n"
                    "{\"type\":\"node\",\"id\":-122,\"lat\":0.002771,\"lon\":0.0002939},\n"
                    "{\"type\":\"node\",\"id\":-123,\"lat\":0.003147,\"lon\":0.0002939},\n"
                    "{\"type\":\"node\",\"id\":-124,\"lat\":0.001805,\"lon\":-0.0006342},\n"
                    "{\"type\":\"node\",\"id\":-125,\"lat\":0.001304,\"lon\":-0.0004712},\n"
                    "{\"type\":\"node\",\"id\":-126,\"lat\":0.001793,\"lon\":-0.001073},\n"
                    "{\"type\":\"node\",\"id\":-127,\"lat\":0.001843,\"lon\":0.000482},\n"
                    "{\"type\":\"node\",\"id\":-128,\"lat\":0.002533,\"lon\":-0.0006342},\n"
                    "{\"type\":\"way\",\"id\":-18,\"nodes\":[-118,-128,-124,-126,-118],\"tags\":{\"building:part\":\"yes\",\"error:circular\":\"15\"},\n"
                    "{\"type\":\"way\",\"id\":-17,\"nodes\":[-119,-112,-114,-127,-119],\"tags\":{\"building:part\":\"yes\",\"error:circular\":\"15\"},\n"
                    "{\"type\":\"way\",\"id\":-16,\"nodes\":[-122,-113,-121,-123,-122],\"tags\":{\"building:part\":\"yes\",\"error:circular\":\"15\"},\n"
                    "{\"type\":\"way\",\"id\":-15,\"nodes\":[-116,-115,-117,-125,-116],\"tags\":{\"building:part\":\"yes\",\"error:circular\":\"15\"},\n"
                    "{\"type\":\"relation\",\"id\":-1,\"members\":[\n"
                    "{\"type\":\"way\",\"ref\":-18,\"role\":\"part\"},\n"
                    "{\"type\":\"way\",\"ref\":-17,\"role\":\"part\"},\n"
                    "{\"type\":\"way\",\"ref\":-16,\"role\":\"part\"},\n"
                    "{\"type\":\"way\",\"ref\":-15,\"role\":\"part\"}],\"tags\":{\"note\":\"Toy Scenario 2\",\"poi\":\"yes\",\"alt_name\":\"B;C;D;E;F\",\"building\":\"yes\",\"name\":\"A\",\"error:circular\":\"15\"}]\n"
                    "}\n",
      OsmJsonWriter(4).toString(map));
  }

  void toyScenario3Test()
  {
    OsmMap::resetCounters();
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::getInstance().read(map, "test-files/conflate/PoiBuildingA.osm", false,
                                            Status::Unknown1);
    OsmMapReaderFactory::getInstance().read(map, "test-files/conflate/PoiBuildingB.osm", false,
                                            Status::Unknown2);

    set< pair<ElementId, ElementId> > s = _addPairs("Toy Scenario 3", map);

    PoiPolygonMerger uut(s);
    vector< pair<ElementId, ElementId> > replaced;
    uut.apply(map, replaced);

    //OsmMapWriterFactory::getInstance().write(map, "test-output/conflate/PoiBuildingToy3.osm");
    HOOT_STR_EQUALS("[7]{(Way:-14, Relation:-1), (Way:-13, Relation:-1), (Way:-12, Relation:-1), (Way:-11, Relation:-1), (Way:-4, Relation:-1), (Node:-38, Relation:-1), (Node:-83, Relation:-1)}",
                    replaced);
    HOOT_STR_EQUALS("{\"version\": 0.6,\"generator\": \"Hootenanny\",\"elements\": [\n"
                    "{\"type\":\"node\",\"id\":-96,\"lat\":0.0009777,\"lon\":0.002351},\n"
                    "{\"type\":\"node\",\"id\":-97,\"lat\":0.0009652,\"lon\":0.003166},\n"
                    "{\"type\":\"node\",\"id\":-98,\"lat\":0.001354,\"lon\":0.003153},\n"
                    "{\"type\":\"node\",\"id\":-99,\"lat\":0.001354,\"lon\":0.002376},\n"
                    "{\"type\":\"node\",\"id\":-100,\"lat\":0.001881,\"lon\":0.003304},\n"
                    "{\"type\":\"node\",\"id\":-101,\"lat\":0.001856,\"lon\":0.00378},\n"
                    "{\"type\":\"node\",\"id\":-102,\"lat\":0.002633,\"lon\":0.003793},\n"
                    "{\"type\":\"node\",\"id\":-103,\"lat\":0.002633,\"lon\":0.003316},\n"
                    "{\"type\":\"node\",\"id\":-104,\"lat\":0.003185,\"lon\":0.003116},\n"
                    "{\"type\":\"node\",\"id\":-105,\"lat\":0.003185,\"lon\":0.002288},\n"
                    "{\"type\":\"node\",\"id\":-106,\"lat\":0.002809,\"lon\":0.002288},\n"
                    "{\"type\":\"node\",\"id\":-107,\"lat\":0.002809,\"lon\":0.003116},\n"
                    "{\"type\":\"node\",\"id\":-108,\"lat\":0.001831,\"lon\":0.001749},\n"
                    "{\"type\":\"node\",\"id\":-109,\"lat\":0.001843,\"lon\":0.002188},\n"
                    "{\"type\":\"node\",\"id\":-110,\"lat\":0.00257,\"lon\":0.002188},\n"
                    "{\"type\":\"node\",\"id\":-111,\"lat\":0.002558,\"lon\":0.001736},\n"
                    "{\"type\":\"way\",\"id\":-14,\"nodes\":[-111,-110,-109,-108,-111],\"tags\":{\"building:part\":\"yes\",\"error:circular\":\"15\"},\n"
                    "{\"type\":\"way\",\"id\":-13,\"nodes\":[-107,-106,-105,-104,-107],\"tags\":{\"building:part\":\"yes\",\"error:circular\":\"15\"},\n"
                    "{\"type\":\"way\",\"id\":-12,\"nodes\":[-103,-102,-101,-100,-103],\"tags\":{\"building:part\":\"yes\",\"error:circular\":\"15\"},\n"
                    "{\"type\":\"way\",\"id\":-11,\"nodes\":[-99,-98,-97,-96,-99],\"tags\":{\"building:part\":\"yes\",\"error:circular\":\"15\"},\n"
                    "{\"type\":\"relation\",\"id\":-1,\"members\":[\n"
                    "{\"type\":\"way\",\"ref\":-14,\"role\":\"part\"},\n"
                    "{\"type\":\"way\",\"ref\":-13,\"role\":\"part\"},\n"
                    "{\"type\":\"way\",\"ref\":-12,\"role\":\"part\"},\n"
                    "{\"type\":\"way\",\"ref\":-11,\"role\":\"part\"}],\"tags\":{\"note\":\"Toy Scenario 3\",\"poi\":\"yes\",\"alt_name\":\"A;B;C;D;E;F\",\"building\":\"yes\",\"name\":\"G\",\"error:circular\":\"15\"}]\n"
                    "}\n",
      OsmJsonWriter(4).toString(map));
  }

  void toyScenario4Test()
  {
    OsmMap::resetCounters();
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::getInstance().read(map, "test-files/conflate/PoiBuildingA.osm", false,
                                            Status::Unknown1);
    OsmMapReaderFactory::getInstance().read(map, "test-files/conflate/PoiBuildingB.osm", false,
                                            Status::Unknown2);

    set< pair<ElementId, ElementId> > s = _addPairs("Toy Scenario 4", map);

    PoiPolygonMerger uut(s);
    vector< pair<ElementId, ElementId> > replaced;
    uut.apply(map, replaced);

    //OsmMapWriterFactory::getInstance().write(map, "test-output/conflate/PoiBuildingToy4.osm");
    HOOT_STR_EQUALS("[6]{(Way:-10, Relation:-1), (Way:-9, Relation:-1), (Way:-8, Relation:-1), (Way:-3, Relation:-1), (Node:-13, Relation:-1), (Node:-82, Relation:-1)}",
                    replaced);
    HOOT_STR_EQUALS("{\"version\": 0.6,\"generator\": \"Hootenanny\",\"elements\": [\n"
                    "{\"type\":\"node\",\"id\":-84,\"lat\":-0.001932,\"lon\":-0.003431},\n"
                    "{\"type\":\"node\",\"id\":-85,\"lat\":-0.001944,\"lon\":-0.002616},\n"
                    "{\"type\":\"node\",\"id\":-86,\"lat\":-0.001556,\"lon\":-0.002628},\n"
                    "{\"type\":\"node\",\"id\":-87,\"lat\":-0.001556,\"lon\":-0.003406},\n"
                    "{\"type\":\"node\",\"id\":-88,\"lat\":-0.001393,\"lon\":-0.002478},\n"
                    "{\"type\":\"node\",\"id\":-89,\"lat\":-0.001418,\"lon\":-0.002001},\n"
                    "{\"type\":\"node\",\"id\":-90,\"lat\":-0.0006401,\"lon\":-0.001989},\n"
                    "{\"type\":\"node\",\"id\":-91,\"lat\":-0.0006401,\"lon\":-0.002465},\n"
                    "{\"type\":\"node\",\"id\":-92,\"lat\":-8.829e-05,\"lon\":-0.002666},\n"
                    "{\"type\":\"node\",\"id\":-93,\"lat\":-8.829e-05,\"lon\":-0.003494},\n"
                    "{\"type\":\"node\",\"id\":-94,\"lat\":-0.0004645,\"lon\":-0.003494},\n"
                    "{\"type\":\"node\",\"id\":-95,\"lat\":-0.0004645,\"lon\":-0.002666},\n"
                    "{\"type\":\"way\",\"id\":-10,\"nodes\":[-95,-94,-93,-92,-95],\"tags\":{\"building:part\":\"yes\",\"error:circular\":\"15\"},\n"
                    "{\"type\":\"way\",\"id\":-9,\"nodes\":[-91,-90,-89,-88,-91],\"tags\":{\"building:part\":\"yes\",\"error:circular\":\"15\"},\n"
                    "{\"type\":\"way\",\"id\":-8,\"nodes\":[-87,-86,-85,-84,-87],\"tags\":{\"building:part\":\"yes\",\"error:circular\":\"15\"},\n"
                    "{\"type\":\"relation\",\"id\":-1,\"members\":[\n"
                    "{\"type\":\"way\",\"ref\":-10,\"role\":\"part\"},\n"
                    "{\"type\":\"way\",\"ref\":-9,\"role\":\"part\"},\n"
                    "{\"type\":\"way\",\"ref\":-8,\"role\":\"part\"}],\"tags\":{\"note\":\"Toy Scenario 4\",\"poi\":\"yes\",\"alt_name\":\"A;B;C;E;F;bar;foo\",\"building\":\"yes\",\"name\":\"G\",\"error:circular\":\"15\"}]\n"
                    "}\n",
      OsmJsonWriter(4).toString(map));
  }

  void toyScenario5Test()
  {
    OsmMap::resetCounters();
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::getInstance().read(map, "test-files/conflate/PoiBuildingA.osm", false,
                                            Status::Unknown1);
    OsmMapReaderFactory::getInstance().read(map, "test-files/conflate/PoiBuildingB.osm", false,
                                            Status::Unknown2);

    set< pair<ElementId, ElementId> > s = _addPairs("Toy Scenario 5", map);
    // remove the dummy road that is in the test. This isn't getting merged.
    s.erase(pair<ElementId, ElementId>(ElementId::way(-2), ElementId::way(-7)));

    PoiPolygonMerger uut(s);
    vector< pair<ElementId, ElementId> > replaced;
    uut.apply(map, replaced);

    //OsmMapWriterFactory::getInstance().write(map, "test-output/conflate/PoiBuildingToy5.osm");
    HOOT_STR_EQUALS("[1]{(Node:-81, Way:-2)}",
                    replaced);
    HOOT_STR_EQUALS("{\"version\": 0.6,\"generator\": \"Hootenanny\",\"elements\": [\n"
                    "{\"type\":\"node\",\"id\":-14,\"lat\":-0.0005151,\"lon\":-0.0005625},\n"
                    "{\"type\":\"node\",\"id\":-15,\"lat\":-0.0005143,\"lon\":-0.001151},\n"
                    "{\"type\":\"node\",\"id\":-16,\"lat\":-0.001492,\"lon\":-0.001152},\n"
                    "{\"type\":\"node\",\"id\":-17,\"lat\":-0.001493,\"lon\":-0.0005618},\n"
                    "{\"type\":\"node\",\"id\":-18,\"lat\":-0.001937,\"lon\":-0.0005624},\n"
                    "{\"type\":\"node\",\"id\":-19,\"lat\":-0.001939,\"lon\":0.0004158},\n"
                    "{\"type\":\"node\",\"id\":-20,\"lat\":-0.001442,\"lon\":0.0004165},\n"
                    "{\"type\":\"node\",\"id\":-21,\"lat\":-0.001443,\"lon\":0.001061},\n"
                    "{\"type\":\"node\",\"id\":-22,\"lat\":-0.0004838,\"lon\":0.001063},\n"
                    "{\"type\":\"node\",\"id\":-23,\"lat\":-0.0004828,\"lon\":0.0003969},\n"
                    "{\"type\":\"node\",\"id\":-24,\"lat\":8.033e-05,\"lon\":0.0003977},\n"
                    "{\"type\":\"node\",\"id\":-25,\"lat\":8.174e-05,\"lon\":-0.0005617},\n"
                    "{\"type\":\"node\",\"id\":-76,\"lat\":-0.002703,\"lon\":-0.001282},\n"
                    "{\"type\":\"node\",\"id\":-77,\"lat\":-0.002331,\"lon\":-0.0003015},\n"
                    "{\"type\":\"node\",\"id\":-78,\"lat\":-0.001832,\"lon\":0.0001057},\n"
                    "{\"type\":\"node\",\"id\":-79,\"lat\":-0.0003266,\"lon\":-0.0003081},\n"
                    "{\"type\":\"node\",\"id\":-80,\"lat\":0.0004134,\"lon\":-0.0009227},\n"
                    "{\"type\":\"node\",\"id\":-81,\"lat\":-0.0009662,\"lon\":1.795e-05},\n"
                    "{\"type\":\"way\",\"id\":-7,\"nodes\":[-80,-79,-81,-78,-77,-76],\"tags\":{\"note\":\"Toy Scenario 5\",\"error:circular\":\"15\"},\n"
                    "{\"type\":\"way\",\"id\":-2,\"nodes\":[-25,-24,-23,-22,-21,-20,-19,-18,-17,-16,-15,-14,-25],\"tags\":{\"note\":\"Toy Scenario 5\",\"poi\":\"yes\",\"alt_name\":\"F\",\"building\":\"yes\",\"name\":\"A\",\"error:circular\":\"15\"}]\n"
                    "}\n",
                    OsmJsonWriter(4).toString(map));
  }

  void toyScenario6Test()
  {
    OsmMap::resetCounters();
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::getInstance().read(map, "test-files/conflate/PoiBuildingA.osm", false,
                                            Status::Unknown1);
    OsmMapReaderFactory::getInstance().read(map, "test-files/conflate/PoiBuildingB.osm", false,
                                            Status::Unknown2);

    set< pair<ElementId, ElementId> > s = _addPairs("Toy Scenario 6", map);

    PoiPolygonMerger uut(s);
    vector< pair<ElementId, ElementId> > replaced;
    uut.apply(map, replaced);

    //OsmMapWriterFactory::getInstance().write(map, "test-output/conflate/PoiBuildingToy6.osm");
    HOOT_STR_EQUALS("[1]{(Node:-75, Way:-1)}", replaced);
    HOOT_STR_EQUALS("{\"version\": 0.6,\"generator\": \"Hootenanny\",\"elements\": [\n"
                    "{\"type\":\"node\",\"id\":-1,\"lat\":-0.0004775,\"lon\":0.00236},\n"
                    "{\"type\":\"node\",\"id\":-2,\"lat\":-0.0004766,\"lon\":0.001771},\n"
                    "{\"type\":\"node\",\"id\":-3,\"lat\":-0.001455,\"lon\":0.00177},\n"
                    "{\"type\":\"node\",\"id\":-4,\"lat\":-0.001456,\"lon\":0.00236},\n"
                    "{\"type\":\"node\",\"id\":-5,\"lat\":-0.0019,\"lon\":0.00236},\n"
                    "{\"type\":\"node\",\"id\":-6,\"lat\":-0.001901,\"lon\":0.003338},\n"
                    "{\"type\":\"node\",\"id\":-7,\"lat\":-0.001405,\"lon\":0.003339},\n"
                    "{\"type\":\"node\",\"id\":-8,\"lat\":-0.001406,\"lon\":0.003983},\n"
                    "{\"type\":\"node\",\"id\":-9,\"lat\":-0.0004462,\"lon\":0.003985},\n"
                    "{\"type\":\"node\",\"id\":-10,\"lat\":-0.0004452,\"lon\":0.003319},\n"
                    "{\"type\":\"node\",\"id\":-11,\"lat\":0.0001179,\"lon\":0.00332},\n"
                    "{\"type\":\"node\",\"id\":-12,\"lat\":0.0001194,\"lon\":0.00236},\n"
                    "{\"type\":\"way\",\"id\":-1,\"nodes\":[-12,-11,-10,-9,-8,-7,-6,-5,-4,-3,-2,-1,-12],\"tags\":{\"note\":\"Toy Scenario 6\",\"poi\":\"yes\",\"alt_name\":\"F\",\"building\":\"yes\",\"name\":\"A\",\"error:circular\":\"15\"}]\n"
                    "}\n",
      OsmJsonWriter(4).toString(map));
  }

  void mergeWayAsPolyTest()
  {
    OsmMap::resetCounters();
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, "test-files/conflate/poi-polygon/poi-poly-way-poly-in.osm", false, Status::Unknown1);

    PoiPolygonMerger::merge(map);

    QDir().mkdir("test-output/conflate/poi-polygon");
    MapProjector::projectToWgs84(map);
    OsmMapWriterFactory::getInstance().write(map,
      "test-output/conflate/poi-polygon/poi-poly-way-poly-out.osm");

    HOOT_FILE_EQUALS(
      "test-files/conflate/poi-polygon/poi-poly-way-poly-out.osm",
      "test-output/conflate/poi-polygon/poi-poly-way-poly-out.osm");
  }

  void mergeRelationAsPolyTest()
  {
    OsmMap::resetCounters();
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, "test-files/conflate/poi-polygon/poi-poly-relation-poly-in.osm", false,
      Status::Unknown1);

    PoiPolygonMerger::merge(map);

    QDir().mkdir("test-output/conflate/poi-polygon");
    MapProjector::projectToWgs84(map);
    OsmMapWriterFactory::getInstance().write(map,
      "test-output/conflate/poi-polygon/poi-poly-relation-poly-out.osm");

    HOOT_FILE_EQUALS(
      "test-files/conflate/poi-polygon/poi-poly-relation-poly-out.osm",
      "test-output/conflate/poi-polygon/poi-poly-relation-poly-out.osm");
  }

  void mergeMissingPoiInputTest()
  {
    QString exceptionMsg("");
    try
    {
      OsmMapPtr map(new OsmMap());
      OsmMapReaderFactory::read(
        map, "test-files/conflate/poi-polygon/poi-poly-missing-poi-in.osm", false,
        Status::Unknown1);

      PoiPolygonMerger::merge(map);
    }
    catch (HootException e)
    {
      exceptionMsg = e.what();
    }
    HOOT_STR_EQUALS("No POI passed to POI/Polygon merger.", exceptionMsg.toStdString());
  }

  void mergeMissingPolyInputTest()
  {
    QString exceptionMsg("");
    try
    {
      OsmMapPtr map(new OsmMap());
      OsmMapReaderFactory::read(
        map, "test-files/conflate/poi-polygon/poi-poly-missing-poly-in.osm", false,
        Status::Unknown1);

      PoiPolygonMerger::merge(map);
    }
    catch (HootException e)
    {
      exceptionMsg = e.what();
    }
    HOOT_STR_EQUALS("No polygon passed to POI/Polygon merger.", exceptionMsg.toStdString());
  }

  void mergeMoreThanOnePoiInputTest()
  {
    QString exceptionMsg("");
    try
    {
      OsmMapPtr map(new OsmMap());
      OsmMapReaderFactory::read(
        map, "test-files/conflate/poi-polygon/poi-poly-more-than-one-poi-in.osm", false,
        Status::Unknown1);

      PoiPolygonMerger::merge(map);
    }
    catch (HootException e)
    {
      exceptionMsg = e.what();
    }
    HOOT_STR_EQUALS(
      "More than one POI passed to POI/Polygon merger.", exceptionMsg.toStdString());
  }

  void mergeMoreThanOnePolyInputTest()
  {
    QString exceptionMsg("");
    try
    {
      OsmMapPtr map(new OsmMap());
      OsmMapReaderFactory::read(
        map, "test-files/conflate/poi-polygon/poi-poly-more-than-one-poly-in.osm", false,
        Status::Unknown1);

      PoiPolygonMerger::merge(map);
    }
    catch (HootException e)
    {
      exceptionMsg = e.what();
    }
    HOOT_STR_EQUALS(
      "More than one polygon passed to POI/Polygon merger.", exceptionMsg.toStdString());
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PoiPolygonMergerTest, "quick");

}
