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
 * @copyright Copyright (C) 2012, 2013, 2014, 2015, 2016, 2018, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/elements/Tags.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/schema/SchemaChecker.h>
#include <hoot/core/util/Log.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

const double epsilon = 1e-6;

namespace hoot
{

class SchemaCheckerTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(SchemaCheckerTest);
  CPPUNIT_TEST(checkUnknownTypeTest);
  CPPUNIT_TEST(checkEmptyGeometryTest);
  CPPUNIT_TEST_SUITE_END();

public:

  /**
   * Test if SchemaVertex is unknown type.
   */
  void checkUnknownTypeTest()
  {
    OsmSchema uut;

    uut.createTestingGraph();

    SchemaVertex vertex = uut.getTagVertex("shop=homegoods");

    CPPUNIT_ASSERT_EQUAL(false, SchemaChecker::isUnknownVertexType(vertex));

    vertex = uut.getTagVertex("highway=primary");

    CPPUNIT_ASSERT_EQUAL(true, SchemaChecker::isUnknownVertexType(vertex));
  }

  /**
   * Test if SchemaVertex has empty geometroies.
   */
  void checkEmptyGeometryTest()
  {
    OsmSchema uut;

    uut.createTestingGraph();

    SchemaVertex vertex = uut.getTagVertex("shop=homegoods");

    CPPUNIT_ASSERT_EQUAL(false, SchemaChecker::isEmptyGeometry(vertex));

    vertex = uut.getTagVertex("leisure=track");

    CPPUNIT_ASSERT_EQUAL(true, SchemaChecker::isEmptyGeometry(vertex));

  }


};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(SchemaCheckerTest, "quick");

}
