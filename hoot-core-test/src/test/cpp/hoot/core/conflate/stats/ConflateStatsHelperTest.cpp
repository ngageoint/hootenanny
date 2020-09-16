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
 * @copyright Copyright (C) 2014, 2015, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/conflate/stats/ConflateStatsHelper.h>
#include <hoot/core/info/SingleStat.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

namespace hoot
{

class ConflateStatsHelperTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ConflateStatsHelperTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runTest()
  {
    QList<SingleStat> inputStats1;
    inputStats1.append(SingleStat("Total Features", 6.0));
    QList<SingleStat> inputStats2;
    inputStats2.append(SingleStat("Total Features", 4.0));
    QList<SingleStat> outputStats;
    outputStats.append(SingleStat("Total Features", 12.0));
    outputStats.append(SingleStat("Total Conflated Features", 2.0));
    outputStats.append(SingleStat("Total Unmatched Features From Map 1", 1.0));
    outputStats.append(SingleStat("Total Unmatched Features From Map 2", 3.0));
    QList<SingleStat> statsToUpdate;
    statsToUpdate.append(SingleStat("Some Stat", 0.0));

    ConflateStatsHelper uut1(inputStats1, inputStats2, outputStats);
    uut1.updateStats(statsToUpdate);

    CPPUNIT_ASSERT_EQUAL(1, inputStats1.size());
    CPPUNIT_ASSERT_EQUAL(1, inputStats2.size());
    CPPUNIT_ASSERT_EQUAL(4, outputStats.size());
    CPPUNIT_ASSERT_EQUAL(6, statsToUpdate.size());
    CPPUNIT_ASSERT_EQUAL(QString("Some Stat").toStdString(), statsToUpdate.at(0).name.toStdString());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      2.0,
      uut1.getSingleStat(
        "Difference Between Total Features in Output and Total Features in Inputs", statsToUpdate),
      1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      20.0,
      uut1.getSingleStat(
        "Percentage Difference Between Total Features in Output and Total Features in Inputs",
        statsToUpdate),
      1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      33.3,
      uut1.getSingleStat(
        "Percentage of Total Map 1 Features Matched With Map 2 Features", statsToUpdate),
      1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      16.7,
      uut1.getSingleStat(
        "Percentage of Total Map 1 Features Not Matched With Map 2 Features", statsToUpdate),
      1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      75.0,
      uut1.getSingleStat(
        "Percentage of Total Map 2 Features Not Matched With Map 1 Features", statsToUpdate),
      1e-1);

    // TODO: add length/area stats?

    statsToUpdate.clear();
    statsToUpdate.append(SingleStat("Some Stat 1", 0.0));
    statsToUpdate.append(SingleStat("Some Stat 2", 0.0));
    ConflateStatsHelper uut2(inputStats1, inputStats2, outputStats);
    uut2.updateStats(statsToUpdate, 1);

    CPPUNIT_ASSERT_EQUAL(7, statsToUpdate.size());
    CPPUNIT_ASSERT_EQUAL(
      QString("Some Stat 1").toStdString(), statsToUpdate.at(0).name.toStdString());
    CPPUNIT_ASSERT_EQUAL(
      QString("Some Stat 2").toStdString(), statsToUpdate.at(6).name.toStdString());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ConflateStatsHelperTest, "quick");

}

