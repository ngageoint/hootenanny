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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/conflate/poi-polygon/PoiPolygonMerger.h>
#include <hoot/core/elements/ConstElementVisitor.h>
#include <hoot/core/io/OsmJsonWriter.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/schema/OverwriteTagMerger.h>
#include <hoot/core/io/OsmJsonReader.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

/**
 * See "Hootenanny - POI to Building" power point for a description of the tests.
 *
 * PoiPolygonMerger also gets a lot of testing in case tests.
 */
class PoiPolygonMergerTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(PoiPolygonMergerTest);
  CPPUNIT_TEST(basicTest);
  CPPUNIT_TEST(toyScenario1Test);
  CPPUNIT_TEST(toyScenario2Test);
  CPPUNIT_TEST(toyScenario3Test);
  CPPUNIT_TEST(toyScenario4Test);
  CPPUNIT_TEST(toyScenario5Test);
  CPPUNIT_TEST(toyScenario6Test);
  CPPUNIT_TEST_SUITE_END();

public:

  PoiPolygonMergerTest()
    : HootTestFixture("test-files/conflate/poi-polygon/PoiPolygonMergerTest/",
                      "test-output/conflate/poi-polygon/PoiPolygonMergerTest/")
  {
    setResetType(ResetBasic);
  }

  void basicTest()
  {
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
      set<pair<ElementId, ElementId>> s;
      s.insert(pair<ElementId, ElementId>(w1->getElementId(), n1->getElementId()));
      PoiPolygonMerger uut(s);
      uut.setTagMergerClass(QString::fromStdString(OverwriteTag2Merger::className()));
      vector<pair<ElementId, ElementId>> replaced;
      uut.apply(map2, replaced);

      const QString testFileName = "basicTest1.json";
      OsmJsonWriter writer;
      writer.setIncludeCompatibilityTags(false);
      writer.open(_outputPath + testFileName);
      MapProjector::projectToWgs84(map2);
      writer.write(map2);
      writer.close();
      HOOT_FILE_EQUALS(_inputPath + testFileName, _outputPath + testFileName);
    }

    {
      w1->setStatus(Status::Unknown2);
      n1->setStatus(Status::Unknown1);
      OsmMapPtr map2(new OsmMap(map));
      set<pair<ElementId, ElementId>> s;
      s.insert(pair<ElementId, ElementId>(w1->getElementId(), n1->getElementId()));
      PoiPolygonMerger uut(s);
      uut.setTagMergerClass(QString::fromStdString(OverwriteTag2Merger::className()));
      vector<pair<ElementId, ElementId>> replaced;
      uut.apply(map2, replaced);

      const QString testFileName = "basicTest2.json";
      OsmJsonWriter writer;
      writer.setIncludeCompatibilityTags(false);
      writer.open(_outputPath + testFileName);
      MapProjector::projectToWgs84(map2);
      writer.write(map2);
      writer.close();
      HOOT_FILE_EQUALS(_inputPath + testFileName, _outputPath + testFileName);
    }
  }

  class AddPairsVisitor : public ConstElementVisitor
  {
  public:

    AddPairsVisitor(OsmMapPtr map, QString scenario) : _map(map), _scenario(scenario) {}

    set<pair<ElementId, ElementId>> getPairs() const
    {
      set<pair<ElementId, ElementId>> result;

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

    virtual QString getDescription() const {return ""; }
    virtual std::string getClassName() const { return ""; }

  private:

    set<pair<ElementId, ElementId>> _pairs;
    vector<ElementId> _e1, _e2;
    OsmMapPtr _map;
    QString _scenario;
  };

  set<pair<ElementId, ElementId>> _addPairs(QString scenario, OsmMapPtr map)
  {
    AddPairsVisitor v(map, scenario);
    map->visitRw(v);
    return v.getPairs();
  }

  void toyScenario1Test()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, "test-files/conflate/poi-polygon/PoiBuildingA.osm", false, Status::Unknown1);
    OsmMapReaderFactory::read(
      map, "test-files/conflate/poi-polygon/PoiBuildingB.osm", false, Status::Unknown2);

    set<pair<ElementId, ElementId>> s = _addPairs("Toy Scenario 1", map);

    PoiPolygonMerger uut(s);
    vector<pair<ElementId, ElementId>> replaced;
    uut.apply(map, replaced);
    HOOT_STR_EQUALS("[6]{(Way(-22), Relation(-1)), (Way(-21), Relation(-1)), (Way(-20), Relation(-1)), (Way(-19), Relation(-1)), (Way(-6), Relation(-1)), (Node(-145), Relation(-1))}",
                    replaced);

    const QString testFileName = "toyScenario1Test.json";
    OsmJsonWriter writer(4);
    writer.setIncludeCompatibilityTags(false);
    writer.open(_outputPath + testFileName);
    MapProjector::projectToWgs84(map);
    writer.write(map);
    writer.close();
    HOOT_FILE_EQUALS(_inputPath + testFileName, _outputPath + testFileName);
  }

  void toyScenario2Test()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, "test-files/conflate/poi-polygon/PoiBuildingA.osm", false, Status::Unknown1);
    OsmMapReaderFactory::read(
      map, "test-files/conflate/poi-polygon/PoiBuildingB.osm", false, Status::Unknown2);

    set<pair<ElementId, ElementId>> s = _addPairs("Toy Scenario 2", map);

    PoiPolygonMerger uut(s);
    vector<pair<ElementId, ElementId>> replaced;
    uut.apply(map, replaced);
    HOOT_STR_EQUALS("[6]{(Way(-18), Relation(-1)), (Way(-17), Relation(-1)), (Way(-16), Relation(-1)), (Way(-15), Relation(-1)), (Way(-5), Relation(-1)), (Node(-120), Relation(-1))}",
                    replaced);

    const QString testFileName = "toyScenario2Test.json";
    OsmJsonWriter writer(4);
    writer.setIncludeCompatibilityTags(false);
    writer.open(_outputPath + testFileName);
    MapProjector::projectToWgs84(map);
    writer.write(map);
    writer.close();
    HOOT_FILE_EQUALS(_inputPath + testFileName, _outputPath + testFileName);
  }

  void toyScenario3Test()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, "test-files/conflate/poi-polygon/PoiBuildingA.osm", false, Status::Unknown1);
    OsmMapReaderFactory::read(
      map, "test-files/conflate/poi-polygon/PoiBuildingB.osm", false, Status::Unknown2);

    set<pair<ElementId, ElementId>> s = _addPairs("Toy Scenario 3", map);

    PoiPolygonMerger uut(s);
    vector<pair<ElementId, ElementId>> replaced;
    uut.apply(map, replaced);
    HOOT_STR_EQUALS("[7]{(Way(-14), Relation(-1)), (Way(-13), Relation(-1)), (Way(-12), Relation(-1)), (Way(-11), Relation(-1)), (Way(-4), Relation(-1)), (Node(-38), Relation(-1)), (Node(-83), Relation(-1))}",
                    replaced);

    const QString testFileName = "toyScenario3Test.json";
    OsmJsonWriter writer(4);
    writer.setIncludeCompatibilityTags(false);
    writer.open(_outputPath + testFileName);
    MapProjector::projectToWgs84(map);
    writer.write(map);
    writer.close();
    HOOT_FILE_EQUALS(_inputPath + testFileName, _outputPath + testFileName);
  }

  void toyScenario4Test()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, "test-files/conflate/poi-polygon/PoiBuildingA.osm", false, Status::Unknown1);
    OsmMapReaderFactory::read(
      map, "test-files/conflate/poi-polygon/PoiBuildingB.osm", false, Status::Unknown2);

    set<pair<ElementId, ElementId>> s = _addPairs("Toy Scenario 4", map);

    PoiPolygonMerger uut(s);
    vector<pair<ElementId, ElementId>> replaced;
    uut.apply(map, replaced);
    HOOT_STR_EQUALS("[6]{(Way(-10), Relation(-1)), (Way(-9), Relation(-1)), (Way(-8), Relation(-1)), (Way(-3), Relation(-1)), (Node(-13), Relation(-1)), (Node(-82), Relation(-1))}",
                    replaced);

    const QString testFileName = "toyScenario4Test.json";
    OsmJsonWriter writer(4);
    writer.setIncludeCompatibilityTags(false);
    writer.open(_outputPath + testFileName);
    MapProjector::projectToWgs84(map);
    writer.write(map);
    writer.close();
    HOOT_FILE_EQUALS(_inputPath + testFileName, _outputPath + testFileName);
  }

  void toyScenario5Test()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, "test-files/conflate/poi-polygon/PoiBuildingA.osm", false, Status::Unknown1);
    OsmMapReaderFactory::read(
      map, "test-files/conflate/poi-polygon/PoiBuildingB.osm", false, Status::Unknown2);

    set<pair<ElementId, ElementId>> s = _addPairs("Toy Scenario 5", map);
    // remove the dummy road that is in the test. This isn't getting merged.
    s.erase(pair<ElementId, ElementId>(ElementId::way(-2), ElementId::way(-7)));

    PoiPolygonMerger uut(s);
    vector<pair<ElementId, ElementId>> replaced;
    uut.apply(map, replaced);
    HOOT_STR_EQUALS("[1]{(Node(-81), Way(-2))}", replaced);

    const QString testFileName = "toyScenario5Test.json";
    OsmJsonWriter writer(4);
    writer.setIncludeCompatibilityTags(false);
    writer.open(_outputPath + testFileName);
    MapProjector::projectToWgs84(map);
    writer.write(map);
    writer.close();
    HOOT_FILE_EQUALS(_inputPath + testFileName, _outputPath + testFileName);
  }

  void toyScenario6Test()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, "test-files/conflate/poi-polygon/PoiBuildingA.osm", false, Status::Unknown1);
    OsmMapReaderFactory::read(
      map, "test-files/conflate/poi-polygon/PoiBuildingB.osm", false, Status::Unknown2);

    set<pair<ElementId, ElementId>> s = _addPairs("Toy Scenario 6", map);

    PoiPolygonMerger uut(s);
    vector<pair<ElementId, ElementId>> replaced;
    uut.apply(map, replaced);
    HOOT_STR_EQUALS("[1]{(Node(-75), Way(-1))}", replaced);

    const QString testFileName = "toyScenario6Test.json";
    OsmJsonWriter writer(4);
    writer.setIncludeCompatibilityTags(false);
    writer.open(_outputPath + testFileName);
    MapProjector::projectToWgs84(map);
    writer.write(map);
    writer.close();
    HOOT_FILE_EQUALS(_inputPath + testFileName, _outputPath + testFileName);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PoiPolygonMergerTest, "quick");

}
