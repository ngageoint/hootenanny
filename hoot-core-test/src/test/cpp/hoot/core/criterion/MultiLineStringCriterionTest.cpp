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
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/criterion/MultiLineStringCriterion.h>
#include <hoot/core/schema/MetadataTags.h>

namespace hoot
{

class MultiLineStringCriterionTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(MultiLineStringCriterionTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runBasicTest()
  {
    RelationPtr relation = std::make_shared<Relation>(Status::Unknown1, 1);
    MultiLineStringCriterion uut;

    relation->setType(MetadataTags::RelationMultilineString());
    CPPUNIT_ASSERT(uut.isSatisfied(relation));

    relation->setType("");
    CPPUNIT_ASSERT(!uut.isSatisfied(relation));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MultiLineStringCriterionTest, "quick");

}
