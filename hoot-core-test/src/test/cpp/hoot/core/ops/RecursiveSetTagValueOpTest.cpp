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
 * @copyright Copyright (C) 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/ops/RecursiveSetTagValueOp.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/criterion/ElementTypeCriterion.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

namespace hoot
{

class RecursiveSetTagValueOpTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(RecursiveSetTagValueOpTest);
  CPPUNIT_TEST(runRelationTest);
  CPPUNIT_TEST(runNoCritTest);
  CPPUNIT_TEST_SUITE_END();

public:

  RecursiveSetTagValueOpTest() :
  HootTestFixture(
    "test-files/ops/RecursiveSetTagValueOp", "test-output/ops/RecursiveSetTagValueOp")
  {
  }

  // Since this wraps SetTagValueVisitor for tag writing, SetTagValueVisitorTest covers more
  // testing than what we're doing here.

  void runRelationTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, true, true, "test-files/ops/ElementIdToVersionMapper/runBasicTest-in.osm");

    QStringList keys;
    keys.append("test_key");
    keys.append("highway");
    QStringList values;
    values.append("test_val");
    values.append("secondary");

    // Write two tags to the relation and its members only.

    RecursiveSetTagValueOp uut(keys, values, ElementCriterionPtr(new RelationCriterion()));
    uut.apply(map);

    MapProjector::projectToWgs84(map);
    OsmMapWriterFactory::write(map, _outputPath + "/runRelationTest-out.osm");
    HOOT_FILE_EQUALS(
      _inputPath + "/runRelationTest-out.osm", _outputPath + "/runRelationTest-out.osm");
  }

  void runNoCritTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, true, true, "test-files/ops/ElementIdToVersionMapper/runBasicTest-in.osm");

    QStringList keys;
    keys.append("test_key");
    keys.append("highway");
    QStringList values;
    values.append("test_val");
    values.append("secondary");

    // With no crit specified, we end up writing two tags to all elements.

    RecursiveSetTagValueOp uut(keys, values, ElementCriterionPtr());
    uut.apply(map);

    MapProjector::projectToWgs84(map);
    OsmMapWriterFactory::write(map, _outputPath + "/runNoCritTest-out.osm");
    HOOT_FILE_EQUALS(_inputPath + "/runNoCritTest-out.osm", _outputPath + "/runNoCritTest-out.osm");
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(RecursiveSetTagValueOpTest, "quick");

}
