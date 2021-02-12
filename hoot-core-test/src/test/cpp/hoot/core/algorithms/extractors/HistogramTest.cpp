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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019 Maxar (http://www.maxar.com/)
 */

//  Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/extractors/Histogram.h>
#include <hoot/core/util/Log.h>

namespace hoot
{

class HistogramTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(HistogramTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runTest()
  {
    Histogram uut(16);

    uut.addAngle(toRadians(25.0), 10);
    uut.addAngle(toRadians(100.0), 20);
    uut.smooth(M_PI / 8.0);
    uut.normalize();

    HOOT_STR_EQUALS("11.25°: 0.0807461, 33.75°: 0.135935, 56.25°: 0.116651, 78.75°: 0.179311, 101.25°: 0.267439, 123.75°: 0.161358, 146.25°: 0.0359945, 168.75°: 0.00295457, 191.25°: 8.92202e-5, 213.75°: 9.91146e-7, 236.25°: 4.05363e-9, 258.75°: 2.03138e-9, 281.25°: 4.95573e-7, 303.75°: 4.46101e-5, 326.25°: 0.00147729, 348.75°: 0.017998", uut);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(HistogramTest, "quick");

}
