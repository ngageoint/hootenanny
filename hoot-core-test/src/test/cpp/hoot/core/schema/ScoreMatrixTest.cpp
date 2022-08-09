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
 * @copyright Copyright (C) 2013, 2014, 2015, 2018, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/schema/ScoreMatrix.h>

using namespace std;

namespace hoot
{

class ScoreMatrixTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ScoreMatrixTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runTest()
  {
    /*
     *              | <unassigned> | California | Hostel | in  | Hollywood
     * <unassigned> | -            | .8         | 1      | 0.5 | 1
     * Hotel        | 1            | 0.98       | 0.1    | 1   | 0.8
     * California   | 0.8          | 0.0        | 0.98   | 1   | 0.99
     */

    ScoreMatrix<double> uut(5, 3);

    uut.set(0, 1, 1);
    uut.set(0, 2, 0.8);

    uut.set(1, 0, .8);
    uut.set(1, 1, .98);
    uut.set(1, 2, 0.0);

    uut.set(2, 0, 1);
    uut.set(2, 1, .1);
    uut.set(2, 2, .98);

    uut.set(3, 0, .5);
    uut.set(3, 1, 1);
    uut.set(3, 2, 1);

    uut.set(4, 0, 1);
    uut.set(4, 1, .8);
    uut.set(4, 2, .99);

    HOOT_STR_EQUALS(1.6, uut.minSumScore());

    uut.resize(5, 3);

    uut(0, 1) = 1;
    uut(0, 2) = 0.8;

    uut(1, 0) = .8;
    uut(1, 1) = .98;
    uut(1, 2) = 0.0;

    uut(2, 0) = 1;
    uut(2, 1) = .1;
    uut(2, 2) = .98;

    uut(3, 0) = .5;
    uut(3, 1) = 1;
    uut(3, 2) = 1;

    uut(4, 0) = 1;
    uut(4, 1) = .8;
    uut(4, 2) = .99;

    HOOT_STR_EQUALS(1.6, uut.minSumScore());

    HOOT_STR_EQUALS("\n"
      "|   | 0 | 1 | 2 | \n"
      "| 0 |   0 |    1 |  0.8 | \n"
      "| 1 | 0.8 | 0.98 |    0 | \n"
      "| 2 |   1 |  0.1 | 0.98 | \n"
      "| 3 | 0.5 |    1 |    1 | \n"
      "| 4 |   1 |  0.8 | 0.99 | \n",
      uut.toTableString());

    vector<QString> rows( {"<unassigned>", "California", "Hostel", "in", "Hollywood"} );
    vector<QString> columns( {"<unassigned>", "Hotel", "California"} );
    uut.setLabels(rows, columns);

    HOOT_STR_EQUALS("\n"
      "|              | <unassigned> | California | Hotel | \n"
      "| <unassigned> |            0 |        0.8 |     1 | \n"
      "|   California |          0.8 |          0 |  0.98 | \n"
      "|    Hollywood |            1 |       0.99 |   0.8 | \n"
      "|       Hostel |            1 |       0.98 |   0.1 | \n"
      "|           in |          0.5 |          1 |     1 | \n",
      uut.toTableString());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ScoreMatrixTest, "quick");

}

