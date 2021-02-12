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
 * @copyright Copyright (C) 2013, 2014, 2015, 2016, 2017, 2018, 2019, 2020 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/conflate/UnifyingConflator.h>
#include <hoot/core/criterion/TagKeyCriterion.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/scoring/MatchComparator.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/visitors/FilteredVisitor.h>
#include <hoot/core/visitors/AddUuidVisitor.h>
#include <hoot/core/visitors/ElementIdsVisitor.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

using namespace std;

namespace hoot
{

class MatchComparatorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(MatchComparatorTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST_SUITE_END();

public:

  MatchComparatorTest()
    : HootTestFixture("test-files/",
                      UNUSED_PATH)
  {
    setResetType(ResetBasic);
  }

  void runTest()
  {
    DisableLog dl;

    OsmXmlReader reader;

    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read(_inputPath + "ToyBuildingsTestA.osm", map);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read(_inputPath + "ToyBuildingsTestB.osm", map);

    // introduce a false positive in the test data.
    vector<long> wids =
      ElementIdsVisitor::findElementsByTag(map, ElementType::Way, "name", "Cheddar's Casual Cafe");
    map->getWay(wids[0])->getTags()[MetadataTags::Ref1()] = "Bad " + MetadataTags::Ref1();

    // introduce a false negative in the test data.
    wids = ElementIdsVisitor::findElementsByTag(map, ElementType::Way, "name", "Freddy's");
    map->getWay(wids[0])->getTags()[MetadataTags::Ref1()] = "Biondi";

    // add a uuid to all buildings.
    TagKeyCriterion tagKeyCrit(MetadataTags::Ref1(), MetadataTags::Ref2());
    AddUuidVisitor uuid("uuid");
    FilteredVisitor v(tagKeyCrit, uuid);
    map->visitRw(v);

    OsmMapPtr copy(new OsmMap(map));

    UnifyingConflator conflator;
    conflator.apply(copy);

    MatchComparator comparator;
    double tpr = comparator.evaluateMatches(map, copy);
    LOG_TRACE(comparator.toString());

    // for debugging
//    MapProjector::projectToWgs84(copy);
//    FileUtils::makeDir("test-output/scoring");
//    OsmXmlWriter writer;
//    writer.write(copy, "test-output/scoring/MatchComparatorTest.osm");

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.75, tpr, 0.001);
    CPPUNIT_ASSERT_EQUAL(6, comparator.getTp());
    CPPUNIT_ASSERT_EQUAL(1, comparator.getFn());
    CPPUNIT_ASSERT_EQUAL(1, comparator.getFp());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.857143, comparator.getPertyScore(), 0.000001);
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MatchComparatorTest, "quick");

}
