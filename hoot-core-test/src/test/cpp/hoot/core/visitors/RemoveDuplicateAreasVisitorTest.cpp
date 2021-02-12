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

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// geos
#include <geos/io/WKTReader.h>
#include <geos/geom/Point.h>

// hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/visitors/ElementIdsVisitor.h>
#include <hoot/core/visitors/RemoveDuplicateAreasVisitor.h>

// TGS
#include <tgs/Statistics/Random.h>
using namespace Tgs;

namespace hoot
{

class RemoveDuplicateAreasVisitorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(RemoveDuplicateAreasVisitorTest);
  CPPUNIT_TEST(runToyTest);
  CPPUNIT_TEST_SUITE_END();

public:

  RemoveDuplicateAreasVisitorTest()
    : HootTestFixture("test-files/visitors/",
                      UNUSED_PATH)
  {
    setResetType(ResetBasic);
  }

  void runToyTest()
  {
    OsmXmlReader reader;

    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read(_inputPath + "RemoveDuplicateAreasVisitorTest.osm", map);
    MapProjector::projectToPlanar(map);

    RemoveDuplicateAreasVisitor uut;
    map->visitRw(uut);

    // these "duplicates" should not be removed.
    CPPUNIT_ASSERT_EQUAL(
      2ul, ElementIdsVisitor::findElementsByTag(map, ElementType::Way, "note", "tag difference").size());
    CPPUNIT_ASSERT_EQUAL(
      2ul, ElementIdsVisitor::findElementsByTag(map, ElementType::Way, "note", "small difference").size());
    CPPUNIT_ASSERT_EQUAL(
      2ul, ElementIdsVisitor::findElementsByTag(map, ElementType::Way, "note", "different name").size());

    // these duplicates should be removed.
    CPPUNIT_ASSERT_EQUAL(
      1ul, ElementIdsVisitor::findElementsByTag(map, ElementType::Way, "note", "double").size());
    CPPUNIT_ASSERT_EQUAL(
      1ul, ElementIdsVisitor::findElementsByTag(map, ElementType::Way, "note", "triple").size());
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(RemoveDuplicateAreasVisitorTest, "quick");

}


