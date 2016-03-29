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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/conflate/extractors/Histogram.h>

// Standard
#include <math.h>

#include "../../TestUtils.h"

namespace hoot
{

class HistogramTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(HistogramTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST(smoothTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runTest()
  {
    Histogram h(16);

    HOOT_STR_EQUALS(8, h.getBin(M_PI));
    HOOT_STR_EQUALS(7, h.getBin(M_PI - 0.01));
    HOOT_STR_EQUALS(0, h.getBin(0.01));
    HOOT_STR_EQUALS(15, h.getBin(-0.01));
    HOOT_STR_EQUALS(8, h.getBin(-M_PI));
    HOOT_STR_EQUALS(15, h.getBin(M_PI * 2.0 - .001));
    HOOT_STR_EQUALS(0, h.getBin(M_PI * 2.0));
    HOOT_STR_EQUALS(0, h.getBin(M_PI * 2.0 + .001));
  }

  void smoothTest()
  {
    Histogram h(8);
    h.addAngle(M_PI / 4.0, 1.0);
    h.smooth(M_PI / 4);
    HOOT_STR_EQUALS("[8]{0.273035, 0.450158, 0.273035, 0.0609223, 0.00500081, 0.000151011, 0.00500081, 0.0609223}",
      h.getAllBins());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(HistogramTest, "quick");

}
