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
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/conflate/SearchBoundsCalculator.h>

namespace hoot
{

class SearchBoundsCalculatorTest : public HootTestFixture
{
    CPPUNIT_TEST_SUITE(SearchBoundsCalculatorTest);
    CPPUNIT_TEST(runBasicTest);
    CPPUNIT_TEST_SUITE_END();

public:

  SearchBoundsCalculatorTest() :
  HootTestFixture(
    "test-files/conflate/ConflateUtilsTest/", "test-output/conflate/ConflateUtilsTest/")
  {
    //setResetType(ResetAll);
  }

  void runBasicTest()
  {

  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(SearchBoundsCalculatorTest, "quick");

}
