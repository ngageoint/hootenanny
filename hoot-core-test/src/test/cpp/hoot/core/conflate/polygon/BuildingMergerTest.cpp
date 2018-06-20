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
 * @copyright Copyright (C) 2013, 2014, 2015, 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/conflate/polygon/BuildingMerger.h>
#include <hoot/core/elements/ConstElementVisitor.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/io/OsmJsonWriter.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/util/MetadataTags.h>
#include <hoot/core/visitors/FindWaysVisitor.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Qt
#include <QDebug>
#include <QDir>
#include <QBuffer>
#include <QByteArray>

// Tgs
#include <tgs/StreamUtils.h>

#include "../../TestUtils.h"

using namespace std;

namespace hoot
{

class BuildingMergerTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(BuildingMergerTest);
  CPPUNIT_TEST(runMatchTest);
  CPPUNIT_TEST(runTagTest);
  CPPUNIT_TEST(runKeepMoreComplexGeometryWhenAutoMergingTest1);
  CPPUNIT_TEST(runKeepMoreComplexGeometryWhenAutoMergingTest2);
  CPPUNIT_TEST_SUITE_END();

public:

  void setUp()
  {
    TestUtils::mkpath("test-output/conflate/polygon");
  }

  ConstWayPtr getWay(ConstOsmMapPtr map, const QString& key, const QString& value)
  {
    std::vector<long> wids = FindWaysVisitor::findWaysByTag(map, key, value);
    CPPUNIT_ASSERT_EQUAL((size_t)1, wids.size());
    return map->getWay(wids[0]);
  }

  class RemoveMissVisitor : public ConstElementVisitor
  {
  public:

    RemoveMissVisitor(OsmMapPtr map, QString ref) : _map(map), _ref(ref) {}

    virtual void visit(const ConstElementPtr& e)
    {
      if (e->getTags().get(MetadataTags::Ref1()) != _ref && e->getTags().get(MetadataTags::Ref2()) != _ref)
      {
        RecursiveElementRemover(e->getElementId()).apply(_map);
      }
    }

    virtual QString getDescription() const {return ""; }

  private:

    OsmMapPtr _map;
    QString _ref;
  };

  void runMatchTest()
  {
    OsmXmlReader reader;
    OsmMap::resetCounters();
    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/ToyBuildingsTestA.osm", map);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read("test-files/ToyBuildingsTestB.osm", map);
    MapProjector::projectToPlanar(map);

    vector<long> wids1 = FindWaysVisitor::findWaysByTag(map, MetadataTags::Ref1(), "Target");
    vector<long> wids2 = FindWaysVisitor::findWaysByTag(map, MetadataTags::Ref2(), "Target");
    set< pair<ElementId, ElementId> > pairs;

    for (size_t i = 0; i < wids2.size(); i++)
    {
      pairs.insert(pair<ElementId, ElementId>(ElementId::way(wids1[0]), ElementId::way(wids2[i])));
    }

    BuildingMerger bm(pairs);
    vector< pair<ElementId, ElementId> > replaced;
    bm.apply(map, replaced);

    MapProjector::projectToWgs84(map);
    OsmXmlWriter writer;
    writer.write(map, "test-output/conflate/polygon/BuildingMergerTest-runMatchTest.osm");

    HOOT_STR_EQUALS("[3]{(Way:-15, Way:-7), (Way:-14, Way:-7), (Way:-13, Way:-7)}", replaced);
  }

  void runTagTest()
  {
    OsmXmlReader reader;

    OsmMap::resetCounters();
    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/conflate/unified/AllDataTypesA.osm", map);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read("test-files/conflate/unified/AllDataTypesB.osm", map);

    vector<long> wids1 = FindWaysVisitor::findWaysByTag(map, MetadataTags::Ref1(), "Panera");
    vector<long> wids2 = FindWaysVisitor::findWaysByTag(map, MetadataTags::Ref2(), "Panera");
    set< pair<ElementId, ElementId> > pairs;

    for (size_t i = 0; i < wids2.size(); i++)
    {
      pairs.insert(pair<ElementId, ElementId>(ElementId::way(wids1[0]), ElementId::way(wids2[i])));
    }

    RemoveMissVisitor v(map, "Panera");
    map->visitRw(v);

    vector< pair<ElementId, ElementId> > replaced;

    BuildingMerger bm(pairs);
    bm.apply(map, replaced);

    HOOT_STR_EQUALS("[3]{(Way:-26, Relation:-1), (Way:-25, Relation:-1), (Way:-14, Relation:-1)}",
                    replaced);
    HOOT_STR_EQUALS("{\"version\": 0.6,\"generator\": \"Hootenanny\",\"elements\": [\n"
                    "{\"type\":\"node\",\"id\":-218,\"lat\":39.593278,\"lon\":-104.80656},\n"
                    "{\"type\":\"node\",\"id\":-219,\"lat\":39.593114,\"lon\":-104.80653},\n"
                    "{\"type\":\"node\",\"id\":-220,\"lat\":39.593124,\"lon\":-104.80645},\n"
                    "{\"type\":\"node\",\"id\":-221,\"lat\":39.593106,\"lon\":-104.80644},\n"
                    "{\"type\":\"node\",\"id\":-222,\"lat\":39.593114,\"lon\":-104.8064},\n"
                    "{\"type\":\"node\",\"id\":-223,\"lat\":39.593115,\"lon\":-104.80635},\n"
                    "{\"type\":\"node\",\"id\":-224,\"lat\":39.593291,\"lon\":-104.80637},\n"
                    "{\"type\":\"node\",\"id\":-225,\"lat\":39.593307,\"lon\":-104.80638},\n"
                    "{\"type\":\"node\",\"id\":-226,\"lat\":39.593279,\"lon\":-104.80647},\n"
                    "{\"type\":\"node\",\"id\":-227,\"lat\":39.593297,\"lon\":-104.80625},\n"
                    "{\"type\":\"node\",\"id\":-228,\"lat\":39.593303,\"lon\":-104.80612},\n"
                    "{\"type\":\"node\",\"id\":-229,\"lat\":39.593127,\"lon\":-104.80609},\n"
                    "{\"type\":\"node\",\"id\":-230,\"lat\":39.593124,\"lon\":-104.80611},\n"
                    "{\"type\":\"node\",\"id\":-231,\"lat\":39.593152,\"lon\":-104.80613},\n"
                    "{\"type\":\"node\",\"id\":-232,\"lat\":39.593143,\"lon\":-104.80622},\n"
                    "{\"type\":\"node\",\"id\":-233,\"lat\":39.593122,\"lon\":-104.80621},\n"
                    "{\"type\":\"way\",\"id\":-26,\"nodes\":[-224,-227,-228,-229,-230,-231,-232,-233,-223,-224],\"tags\":{\"" + MetadataTags::BuildingPart() + "\":\"yes\",\"" + MetadataTags::ErrorCircular() + "\":\"15\"},\n"
                    "{\"type\":\"way\",\"id\":-25,\"nodes\":[-218,-219,-220,-221,-222,-223,-224,-225,-226,-218],\"tags\":{\"" + MetadataTags::BuildingPart() + "\":\"yes\",\"" + MetadataTags::ErrorCircular() + "\":\"15\"},\n"
                    "{\"type\":\"relation\",\"id\":-1,\"members\":[\n"
                    "{\"type\":\"way\",\"ref\":-26,\"role\":\"part\"},\n"
                    "{\"type\":\"way\",\"ref\":-25,\"role\":\"part\"}],\"tags\":{\"" + MetadataTags::Ref1() + "\":\"Panera\",\"" + MetadataTags::Ref2() + "\":\"Panera\",\"" + MetadataTags::HootBuildingMatch() + "\":\"true\",\"alt_name\":\"Maid-Rite;Maid-Rite Diner\",\"building\":\"yes\",\"name\":\"Panera Bread\",\"" + MetadataTags::ErrorCircular() + "\":\"15\"}]\n"
                    "}\n",
                    OsmJsonWriter(8).toString(map));
  }

  void runKeepMoreComplexGeometryWhenAutoMergingTest1()
  {
    OsmMap::resetCounters();
    OsmMapPtr map(new OsmMap());
    set< pair<ElementId, ElementId> > pairs = getPairsForComplexAutoMergingTests(map);

    BuildingMerger bm(pairs);
    bm.setKeepMoreComplexGeometryWhenAutoMerging(true);
    vector< pair<ElementId, ElementId> > replaced;
    bm.apply(map, replaced);

    MapProjector::projectToWgs84(map);
    OsmXmlWriter writer;
    writer.write(
      map,
      "test-output/conflate/polygon/BuildingMergerTest-runKeepMoreComplexGeometryWhenAutoMergingTest-true.osm");
    HOOT_STR_EQUALS(
      "[3]{(Way:-18, Relation:-1), (Way:-17, Relation:-1), (Way:-1, Relation:-1)}", replaced);
  }

  void runKeepMoreComplexGeometryWhenAutoMergingTest2()
  {
    OsmMap::resetCounters();
    OsmMapPtr map(new OsmMap());
    set< pair<ElementId, ElementId> > pairs = getPairsForComplexAutoMergingTests(map);

    BuildingMerger bm(pairs);
    bm.setKeepMoreComplexGeometryWhenAutoMerging(false);
    vector< pair<ElementId, ElementId> > replaced;
    bm.apply(map, replaced);

    MapProjector::projectToWgs84(map);
    OsmXmlWriter writer;
    writer.write(
      map,
      "test-output/conflate/polygon/BuildingMergerTest-runKeepMoreComplexGeometryWhenAutoMergingTest-false.osm");
    HOOT_STR_EQUALS("[2]{(Way:-18, Way:-1), (Way:-17, Way:-1)}", replaced);
  }

private:

  set< pair<ElementId, ElementId> > getPairsForComplexAutoMergingTests(OsmMapPtr map)
  {
    OsmXmlReader reader;
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/ToyBuildingsTestA.osm", map);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read("test-files/ToyBuildingsTestB.osm", map);
    MapProjector::projectToPlanar(map);

    vector<long> wids1 = FindWaysVisitor::findWaysByTag(map, MetadataTags::Ref1(), "Panera");
    vector<long> wids2 = FindWaysVisitor::findWaysByTag(map, MetadataTags::Ref2(), "Panera");
    vector<long> wids3 = FindWaysVisitor::findWaysByTag(map, MetadataTags::Ref2(), "Maid-Rite");
    wids2.insert(wids2.end(), wids3.begin(), wids3.end());
    set< pair<ElementId, ElementId> > pairs;

    for (size_t i = 0; i < wids2.size(); i++)
    {
      pairs.insert(pair<ElementId, ElementId>(ElementId::way(wids1[0]), ElementId::way(wids2[i])));
    }

    return pairs;
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(BuildingMergerTest, "quick");

}
