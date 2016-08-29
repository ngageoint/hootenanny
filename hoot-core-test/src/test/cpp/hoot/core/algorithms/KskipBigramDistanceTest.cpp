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
#include <hoot/core/algorithms/Translator.h>
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
    KskipBigramDistance kskip0(0);
    KskipBigramDistance kskip1(1);
    KskipBigramDistance kskip2(2);
    KskipBigramDistance kskip3(3);

    HOOT_STR_EQUALS("ai | ee | et | in | ma | re | st | tr",
                    joinBigrams(kskip0.getBigrams("Main Street")));
    HOOT_STR_EQUALS("ai | an | ee | et | in | ma | mi | re | sr | st | te | tr",
                    joinBigrams(kskip1.getBigrams("Main Street")));
    HOOT_STR_EQUALS("ai | an | ee | et | in | ma | mi | mn | re | rt | se | sr | st | te | tr",
                    joinBigrams(kskip2.getBigrams("Main Street")));
    HOOT_STR_EQUALS("ai | an | ee | et | in | ma | mi | mn | re | rt | se | sr | st | te | tr | tt",
                    joinBigrams(kskip3.getBigrams("Main Street")));

    HOOT_STR_EQUALS("ai | an | ee | es | et | in | ma | mi | re | sr | st | te | tr | we | ws",
                    joinBigrams(kskip1.getBigrams("West Main Street")));
    HOOT_STR_EQUALS("ai | an | ee | es | et | in | ma | mi | mn | re | rt | se | sr | st | te | tr | we | ws | wt",
                    joinBigrams(kskip2.getBigrams("West Main Street")));

    HOOT_STR_EQUALS("ay | gh | gw | ha | hg | hi | hw | ig | ih | wa | wy",
                    joinBigrams(kskip1.getBigrams("HIGHWAY")));
    HOOT_STR_EQUALS("ay | ga | gh | gw | ha | hg | hh | hi | hw | hy | ig | ih | iw | wa | wy",
                    joinBigrams(kskip2.getBigrams("HIGHWAY")));

    HOOT_STR_EQUALS("", joinBigrams(kskip1.getBigrams("")));
    HOOT_STR_EQUALS("", joinBigrams(kskip1.getBigrams("a")));

    //  Try some different languages
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.6666, kskip2.compare(te("хелло ворлд"), te("hello world")), 0.0001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.2727, kskip2.compare(te("هالو وورلد"),  te("hello world")), 0.0001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.3684, kskip2.compare(te("הללו וורלד"),    te("hello world")), 0.0001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.4210, kskip2.compare(te("الواثق"),      te("Al Wathiq")),   0.0001);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, kskip1.compare("Main Street", "MAIN STREET"), 0.0001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, kskip2.compare("MAIN STREET", "Main Street"), 0.0001);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.8000, kskip0.compare("Main Street", "West Main Street"), 0.0001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.8000, kskip1.compare("Main Street", "West Main Street"), 0.0001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.7894, kskip2.compare("Main Street", "West Main Street"), 0.0001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.8000, kskip3.compare("Main Street", "West Main Street"), 0.0001);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.3333, kskip1.compare("Main Street", "Second Street"), 0.0001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.3461, kskip2.compare("Main Street", "Second Street"), 0.0001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.3448, kskip3.compare("Main Street", "Second Street"), 0.0001);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5641, kskip1.compare("First Second Third Fourth Fifth Sixth", "First Second Third"), 0.0001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.6041, kskip2.compare("First Second Third Fourth Fifth Sixth", "First Second Third"), 0.0001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.6226, kskip3.compare("First Second Third Fourth Fifth Sixth", "First Second Third"), 0.0001);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.7435, kskip1.compare("First Second Third Fourth Fifth Sixth", "First Second Third Fourth"), 0.0001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.7916, kskip2.compare("First Second Third Fourth Fifth Sixth", "First Second Third Fourth"), 0.0001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.8113, kskip3.compare("First Second Third Fourth Fifth Sixth", "First Second Third Fourth"), 0.0001);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.8717, kskip1.compare("First Second Third Fourth Fifth Sixth", "First Second Third Fourth Fifth"), 0.0001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.8958, kskip2.compare("First Second Third Fourth Fifth Sixth", "First Second Third Fourth Fifth"), 0.0001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.8867, kskip3.compare("First Second Third Fourth Fifth Sixth", "First Second Third Fourth Fifth"), 0.0001);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.3469, kskip2.compare("KALORAMA COMMUNITY GARDEN", "KALORAMA PARK"), 0.0001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.3333, kskip3.compare("KALORAMA COMMUNITY GARDEN", "KALORAMA PARK"), 0.0001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.2857, kskip2.compare("KALORAMA COMMUNITY GARDEN", "KALORAMA RECREATION CENTER"), 0.0001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.3037, kskip3.compare("KALORAMA COMMUNITY GARDEN", "KALORAMA RECREATION CENTER"), 0.0001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.3333, kskip2.compare("KALORAMA RECREATION CENTER", "KALORAMA PARK"), 0.0001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.3392, kskip3.compare("KALORAMA RECREATION CENTER", "KALORAMA PARK"), 0.0001);

    //  100% code coverage
    HOOT_STR_EQUALS("1-skip bi-gram", kskip1.toString());
    HOOT_STR_EQUALS("2-skip bi-gram", kskip2.toString());
  }

  QString te(const char* s)
  {
    return Translator::getInstance().translateStreet(QString::fromUtf8(s));
  }

  QString joinBigrams(QSet<QString> bigrams)
  {
    QStringList list(bigrams.toList());
    list.sort();
    return list.join(" | ");
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(KskipBigramDistanceTest, "quick");

}

