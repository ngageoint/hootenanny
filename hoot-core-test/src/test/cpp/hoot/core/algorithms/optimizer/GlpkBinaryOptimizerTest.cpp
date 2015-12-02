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
 * @copyright Copyright (C) 2013, 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/algorithms/optimizer/GlpkBinaryOptimizer.h>
#include <hoot/core/util/Log.h>

#include "../../TestUtils.h"

namespace hoot
{

class GlpkBinaryOptimizerTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(GlpkBinaryOptimizerTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runTest()
  {
    GlpkBinaryOptimizer uut;

    // this test is approximating a series of matches between A & B vs. X, Y & Z.
    // 0 - AX
    uut.addColumn(0, 0.9);
    // 1 - AY
    uut.addColumn(1, 0.5);
    // 2 - AZ
    uut.addColumn(2, 0.2);
    // 3 - BX
    uut.addColumn(3, 0.7);
    // 4 - BY
    uut.addColumn(4, 0.6);
    // 5 - BZ
    uut.addColumn(5, 0.1);

    uut.addConstraint(0, 1);
    uut.addConstraint(0, 2);
    uut.addConstraint(1, 2);
    uut.addConstraint(0, 3);
    uut.addConstraint(1, 4);
    uut.addConstraint(2, 5);
    uut.addConstraint(3, 4);
    uut.addConstraint(3, 5);
    uut.addConstraint(4, 5);

    vector<size_t> solution;
    HOOT_STR_EQUALS(1.5, uut.solve(solution));
    HOOT_STR_EQUALS("[2]{0, 4}", solution);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(GlpkBinaryOptimizerTest, "quick");

}
