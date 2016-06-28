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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// GEOS
#include <geos/geom/LineString.h>

// Hoot
#include <hoot/core/algorithms/KskipBigramDistance.h>
#include <hoot/core/util/Log.h>

// Qt
#include <QString>

#include "../TestUtils.h"

namespace hoot
{

class KskipBigramDistanceTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(KskipBigramDistanceTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runTest()
  {
    KskipBigramDistance kskip1(1);
    KskipBigramDistance kskip2(2);
    KskipBigramDistance kskip3(3);
    /** Test text taken from example in:
     *  A Closer Look at Skip-gram Modelling
     *  David Guthrie, Ben Allison, Wei Liu, Louise Guthrie, Yorick Wilks
     *  http://homepages.inf.ed.ac.uk/ballison/pdf/lrec_skipgrams.pdf
     */
    HOOT_STR_EQUALS("insurgents killed | "
                    "insurgents in | "
                    "killed in | "
                    "killed ongoing | "
                    "in ongoing | "
                    "in fighting | "
                    "ongoing fighting",
                    kskip1.getBigrams("Insurgents killed in ongoing fighting").join(" | "));
    HOOT_STR_EQUALS("insurgents killed | "
                    "insurgents in | "
                    "insurgents ongoing | "
                    "killed in | "
                    "killed ongoing | "
                    "killed fighting | "
                    "in ongoing | "
                    "in fighting | "
                    "ongoing fighting",
                    kskip2.getBigrams("Insurgents killed in ongoing fighting").join(" | "));
    HOOT_STR_EQUALS("insurgents killed | "
                    "insurgents in | "
                    "insurgents ongoing | "
                    "insurgents fighting | "
                    "killed in | "
                    "killed ongoing | "
                    "killed fighting | "
                    "in ongoing | "
                    "in fighting | "
                    "ongoing fighting",
                    kskip3.getBigrams("Insurgents killed in ongoing fighting").join(" | "));

    HOOT_STR_EQUALS("insurgents", kskip1.getBigrams("INSURGENTS").join(" | "));
    HOOT_STR_EQUALS("insurgents", kskip2.getBigrams("INSURGENTS").join(" | "));

    HOOT_STR_EQUALS("insurgents killed", kskip1.getBigrams("INSURGENTS KILLED").join(" | "));
    HOOT_STR_EQUALS("insurgents killed", kskip2.getBigrams("INSURGENTS KILLED").join(" | "));

    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, kskip1.compare("Main Street", "MAIN STREET"), 0.0001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, kskip2.compare("MAIN STREET", "Main Street"), 0.0001);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.8716, kskip1.compare("Main Street", "West Main Street"), 0.0001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.8716, kskip2.compare("Main Street", "West Main Street"), 0.0001);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, kskip1.compare("Main Street", "Second Street"), 0.0001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, kskip2.compare("Main Street", "Second Street"), 0.0001);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.8716, kskip1.compare("First Second Third Fourth Fifth Sixth", "First Second Third"), 0.0001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.8408, kskip2.compare("First Second Third Fourth Fifth Sixth", "First Second Third"), 0.0001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.8248, kskip3.compare("First Second Third Fourth Fifth Sixth", "First Second Third"), 0.0001);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.9291, kskip1.compare("First Second Third Fourth Fifth Sixth", "First Second Third Fourth"), 0.0001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.9170, kskip2.compare("First Second Third Fourth Fifth Sixth", "First Second Third Fourth"), 0.0001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.8995, kskip3.compare("First Second Third Fourth Fifth Sixth", "First Second Third Fourth"), 0.0001);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.9690, kskip1.compare("First Second Third Fourth Fifth Sixth", "First Second Third Fourth Fifth"), 0.0001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.9646, kskip2.compare("First Second Third Fourth Fifth Sixth", "First Second Third Fourth Fifth"), 0.0001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.9588, kskip3.compare("First Second Third Fourth Fifth Sixth", "First Second Third Fourth Fifth"), 0.0001);

  }

};

CPPUNIT_TEST_SUITE_REGISTRATION(KskipBigramDistanceTest);

}

