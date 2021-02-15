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
 * @copyright Copyright (C) 2013, 2014, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/ops/RemoveEmptyRelationsOp.h>

namespace hoot
{

class RemoveEmptyRelationsOpTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(RemoveEmptyRelationsOpTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  RemoveEmptyRelationsOpTest()
    : HootTestFixture("test-files/ops/RemoveEmptyRelationsOp/",
                      UNUSED_PATH)
  {
    setResetType(ResetBasic);
  }

  void runBasicTest()
  {
    OsmXmlReader reader;
    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read(_inputPath + "input.osm", map);

    RemoveEmptyRelationsOp uut;
    uut.apply(map);

    CPPUNIT_ASSERT_EQUAL(4L, uut.getNumRemoved());
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(RemoveEmptyRelationsOpTest, "quick");

}


