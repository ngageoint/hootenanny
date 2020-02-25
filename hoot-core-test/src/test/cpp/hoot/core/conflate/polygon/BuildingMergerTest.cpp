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
 * @copyright Copyright (C) 2013, 2014, 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
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
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/conflate/polygon/BuildingMerger.h>
#include <hoot/core/elements/ConstElementVisitor.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/io/OsmJsonWriter.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/visitors/ElementIdsVisitor.h>
#include <hoot/core/io/OsmJsonReader.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Tgs
#include <tgs/StreamUtils.h>

using namespace std;

namespace hoot
{

class BuildingMergerTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(BuildingMergerTest);
  CPPUNIT_TEST(runMatchTest);
  CPPUNIT_TEST(runTagTest);
  CPPUNIT_TEST(runKeepMoreComplexGeometryWhenAutoMergingTest1);
  CPPUNIT_TEST(runKeepMoreComplexGeometryWhenAutoMergingTest2);
  CPPUNIT_TEST(runManyToManyMergeTest);
  CPPUNIT_TEST(runChangedTest);
  CPPUNIT_TEST_SUITE_END();

public:

  BuildingMergerTest() :
  HootTestFixture(
    "test-files/conflate/polygon/BuildingMergerTest/",
    "test-output/conflate/polygon/BuildingMergerTest/")
  {
    setResetType(ResetBasic);
  }

  ConstWayPtr getWay(ConstOsmMapPtr map, const QString& key, const QString& value)
  {
    std::vector<long> wids = ElementIdsVisitor::findElementsByTag(map, ElementType::Way, key, value);
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
    virtual std::string getClassName() const { return ""; }

  private:

    OsmMapPtr _map;
    QString _ref;
  };

  void runMatchTest()
  {
    OsmXmlReader reader;
    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/ToyBuildingsTestA.osm", map);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read("test-files/ToyBuildingsTestB.osm", map);
    MapProjector::projectToPlanar(map);

    vector<long> wids1 =
      ElementIdsVisitor::findElementsByTag(map, ElementType::Way, MetadataTags::Ref1(), "Target");
    vector<long> wids2 =
      ElementIdsVisitor::findElementsByTag(map, ElementType::Way, MetadataTags::Ref2(), "Target");
    set<pair<ElementId, ElementId>> pairs;

    for (size_t i = 0; i < wids2.size(); i++)
    {
      pairs.insert(pair<ElementId, ElementId>(ElementId::way(wids1[0]), ElementId::way(wids2[i])));
    }

    BuildingMerger bm(pairs);
    vector<pair<ElementId, ElementId>> replaced;
    bm.apply(map, replaced);

    MapProjector::projectToWgs84(map);
    OsmXmlWriter writer;
    writer.write(map, _outputPath + "BuildingMergerTest-runMatchTest.osm");

    HOOT_STR_EQUALS("[3]{(Way(-15), Way(-7)), (Way(-14), Way(-7)), (Way(-13), Way(-7))}", replaced);
  }

  void runTagTest()
  {
    OsmXmlReader reader;
    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/conflate/unified/AllDataTypesA.osm", map);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read("test-files/conflate/unified/AllDataTypesB.osm", map);

    vector<long> wids1 =
      ElementIdsVisitor::findElementsByTag(map, ElementType::Way, MetadataTags::Ref1(), "Panera");
    vector<long> wids2 =
      ElementIdsVisitor::findElementsByTag(map, ElementType::Way, MetadataTags::Ref2(), "Panera");
    set<pair<ElementId, ElementId>> pairs;

    for (size_t i = 0; i < wids2.size(); i++)
    {
      pairs.insert(pair<ElementId, ElementId>(ElementId::way(wids1[0]), ElementId::way(wids2[i])));
    }

    RemoveMissVisitor v(map, "Panera");
    map->visitRw(v);

    vector<pair<ElementId, ElementId>> replaced;

    BuildingMerger bm(pairs);
    bm.apply(map, replaced);
    HOOT_STR_EQUALS("[3]{(Way(-26), Relation(-1)), (Way(-25), Relation(-1)), (Way(-14), Relation(-1))}",
                    replaced);

    const QString testFileName = "runTagTest.json";
    OsmJsonWriter writer(8);
    writer.setIncludeCompatibilityTags(false);
    writer.open(_outputPath + testFileName);
    MapProjector::projectToWgs84(map);
    writer.write(map);
    writer.close();
    HOOT_FILE_EQUALS(_inputPath + testFileName, _outputPath + testFileName);
  }

  void runKeepMoreComplexGeometryWhenAutoMergingTest1()
  {
    OsmMapPtr map(new OsmMap());
    set<pair<ElementId, ElementId>> pairs = getPairsForComplexAutoMergingTests(map);

    BuildingMerger bm(pairs);
    bm.setKeepMoreComplexGeometryWhenAutoMerging(true);
    vector<pair<ElementId, ElementId>> replaced;
    bm.apply(map, replaced);

    MapProjector::projectToWgs84(map);
    OsmXmlWriter writer;
    writer.write(
      map,
      _outputPath + "BuildingMergerTest-runKeepMoreComplexGeometryWhenAutoMergingTest-true.osm");
    HOOT_STR_EQUALS(
      "[3]{(Way(-18), Relation(-1)), (Way(-17), Relation(-1)), (Way(-1), Relation(-1))}", replaced);
  }

  void runKeepMoreComplexGeometryWhenAutoMergingTest2()
  {
    OsmMapPtr map(new OsmMap());
    set<pair<ElementId, ElementId>> pairs = getPairsForComplexAutoMergingTests(map);

    BuildingMerger bm(pairs);
    bm.setKeepMoreComplexGeometryWhenAutoMerging(false);
    vector<pair<ElementId, ElementId>> replaced;
    bm.apply(map, replaced);

    MapProjector::projectToWgs84(map);
    OsmXmlWriter writer;
    writer.write(
      map,
      _outputPath + "BuildingMergerTest-runKeepMoreComplexGeometryWhenAutoMergingTest-false.osm");
    HOOT_STR_EQUALS("[2]{(Way(-18), Way(-1)), (Way(-17), Way(-1))}", replaced);
  }

  void runManyToManyMergeTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmXmlReader reader;
    const QString inputPath =
      "test-files/cases/attribute/unifying/building-3136-many-to-many-auto-merge-1";
    reader.setDefaultStatus(Status::Unknown1);
    reader.read(inputPath + "/Input1.osm", map);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read(inputPath + "/Input2.osm", map);
    MapProjector::projectToPlanar(map);

    set<pair<ElementId, ElementId>> pairs = getPairsForManyToManyMergingTest();

    BuildingMerger bm(pairs);
    bm.setMergeManyToManyMatches(true);
    vector<pair<ElementId, ElementId>> replaced;
    bm.apply(map, replaced);

    MapProjector::projectToWgs84(map);
    OsmXmlWriter writer;
    writer.write(map, _outputPath + "BuildingMergerTest-runManyToManyMergeTest.osm");
    HOOT_STR_EQUALS(
      "[5]{(Way(-5), Relation(-1)), (Way(-4), Relation(-1)), (Way(-3), Relation(-1)), (Way(-2), Relation(-1)), (Way(-1), Relation(-1))}",
      replaced);
  }

  void runChangedTest()
  {
    BuildingMerger uut;
    uut.setUseChangedReview(true);
    uut.setChangedReviewIouThreshold(0.2);

    set<pair<ElementId, ElementId>> pairs;
    vector<pair<ElementId, ElementId>> replaced;

    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map,
      "test-files/algorithms/extractors/IntersectionOverUnionExtractorTest/IntersectionOverUnionExtractorTest-in.osm");

    pairs.clear();
    replaced.clear();
    ConstElementPtr building7 = TestUtils::getElementWithTag(map, "name", "Building 7");
    ConstElementPtr building8 = TestUtils::getElementWithTag(map, "name", "Building 8");
    pairs.insert(pair<ElementId, ElementId>(building7->getElementId(), building8->getElementId()));
    uut._pairs = pairs;
    uut.apply(map, replaced);
    CPPUNIT_ASSERT(replaced.size() == 0);
    CPPUNIT_ASSERT(uut.getMarkedReviewText().startsWith("Identified as changed"));

    pairs.clear();
    replaced.clear();
    ConstElementPtr building3 = TestUtils::getElementWithTag(map, "name", "Building 3");
    ConstElementPtr building4 = TestUtils::getElementWithTag(map, "name", "Building 4");
    pairs.insert(pair<ElementId, ElementId>(building3->getElementId(), building4->getElementId()));
    uut._pairs = pairs;
    uut.apply(map, replaced);
    CPPUNIT_ASSERT(replaced.size() == 1);
    CPPUNIT_ASSERT(uut.getMarkedReviewText().isEmpty());

    // These particular buildings likely wouldn't ever be matched in the first place and, therefore,
    // wouldn't be passed to the merger. However, using them to test the IoU = 0 case.
    pairs.clear();
    replaced.clear();
    ConstElementPtr building9 = TestUtils::getElementWithTag(map, "name", "Building 9");
    ConstElementPtr building10 = TestUtils::getElementWithTag(map, "name", "Building 10");
    pairs.insert(pair<ElementId, ElementId>(building9->getElementId(), building10->getElementId()));
    uut._pairs = pairs;
    uut.apply(map, replaced);
    CPPUNIT_ASSERT(replaced.size() == 1);
    CPPUNIT_ASSERT(uut.getMarkedReviewText().isEmpty());
  }

private:

  set<pair<ElementId, ElementId>> getPairsForComplexAutoMergingTests(OsmMapPtr map)
  {
    OsmXmlReader reader;
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/ToyBuildingsTestA.osm", map);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read("test-files/ToyBuildingsTestB.osm", map);
    MapProjector::projectToPlanar(map);

    vector<long> wids1 =
      ElementIdsVisitor::findElementsByTag(map, ElementType::Way, MetadataTags::Ref1(), "Panera");
    vector<long> wids2 =
      ElementIdsVisitor::findElementsByTag(map, ElementType::Way, MetadataTags::Ref2(), "Panera");
    vector<long> wids3 =
      ElementIdsVisitor::findElementsByTag(map, ElementType::Way, MetadataTags::Ref2(), "Maid-Rite");
    wids2.insert(wids2.end(), wids3.begin(), wids3.end());
    set<pair<ElementId, ElementId>> pairs;

    for (size_t i = 0; i < wids2.size(); i++)
    {
      pairs.insert(pair<ElementId, ElementId>(ElementId::way(wids1[0]), ElementId::way(wids2[i])));
    }

    return pairs;
  }

  set<pair<ElementId, ElementId>> getPairsForManyToManyMergingTest() const
  {
    set<pair<ElementId, ElementId>> pairs;
    // came up with this by tracing through the actual conflation of the data to see what was being
    // passed into the merger after matching
    pairs.insert(pair<ElementId, ElementId>(ElementId::way(-2), ElementId::way(-5)));
    pairs.insert(pair<ElementId, ElementId>(ElementId::way(-2), ElementId::way(-4)));
    pairs.insert(pair<ElementId, ElementId>(ElementId::way(-2), ElementId::way(-3)));
    pairs.insert(pair<ElementId, ElementId>(ElementId::way(-1), ElementId::way(-3)));
    return pairs;
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(BuildingMergerTest, "quick");

}
