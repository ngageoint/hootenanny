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
 * @copyright Copyright (C) 2014, 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// geos
#include <geos/geom/Geometry.h>

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/geometry/ElementToGeometryConverter.h>
#include <hoot/core/util/Log.h>

using namespace geos::geom;

namespace hoot
{

class GeometryToElementConverterTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(GeometryToElementConverterTest);
  CPPUNIT_TEST(emptyWayTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void emptyWayTest()
  {
    OsmMapPtr map(new OsmMap());
    ElementPtr w(new Way(Status::Unknown1, -1, 0));
    w->getTags()["building"] = "yes";

    ElementToGeometryConverter uut(map);
    std::shared_ptr<Geometry> g = uut.convertToGeometry(w);

    CPPUNIT_ASSERT_EQUAL(true, g->isEmpty());
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(GeometryToElementConverterTest, "quick");

}

