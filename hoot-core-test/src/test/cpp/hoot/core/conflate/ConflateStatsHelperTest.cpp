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
 * @copyright Copyright (C) 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/conflate/ConflateStatsHelper.h>
#include <hoot/core/ops/stats/SingleStat.h>

// Boost
using namespace boost;

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Qt
#include <QDebug>

#include "../TestUtils.h"

namespace hoot
{

class ConflateStatsHelperTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(ConflateStatsHelperTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runTest()
  {
    QList<SingleStat> inputStats1;
    inputStats1.append(SingleStat("Total Feature Count", 6.0));
    QList<SingleStat> inputStats2;
    inputStats2.append(SingleStat("Total Feature Count", 4.0));
    QList<SingleStat> outputStats;
    outputStats.append(SingleStat("Total Feature Count", 12.0));
    QList<SingleStat> statsToUpdate;
    statsToUpdate.append(SingleStat("Some Stat", 0.0));

    ConflateStatsHelper uut1(inputStats1, inputStats2, outputStats);
    uut1.updateStats(statsToUpdate);

    CPPUNIT_ASSERT_EQUAL(1, inputStats1.size());
    CPPUNIT_ASSERT_EQUAL(1, inputStats2.size());
    CPPUNIT_ASSERT_EQUAL(1, outputStats.size());
    CPPUNIT_ASSERT_EQUAL(3, statsToUpdate.size());
    CPPUNIT_ASSERT_EQUAL(QString("Some Stat").toStdString(), statsToUpdate.at(0).name.toStdString());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      2.0,
      uut1.getSingleStat("Difference Between Total Features in Output and Total Features in Inputs", statsToUpdate),
      1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      20.0,
      uut1.getSingleStat("Percentage Difference Between Total Features in Output and Total Features in Inputs", statsToUpdate),
      1e-1);

    statsToUpdate.clear();
    statsToUpdate.append(SingleStat("Some Stat 1", 0.0));
    statsToUpdate.append(SingleStat("Some Stat 2", 0.0));
    ConflateStatsHelper uut2(inputStats1, inputStats2, outputStats);
    uut2.updateStats(statsToUpdate, 1);

    CPPUNIT_ASSERT_EQUAL(4, statsToUpdate.size());
    CPPUNIT_ASSERT_EQUAL(QString("Some Stat 1").toStdString(), statsToUpdate.at(0).name.toStdString());
    CPPUNIT_ASSERT_EQUAL(QString("Some Stat 2").toStdString(), statsToUpdate.at(3).name.toStdString());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ConflateStatsHelperTest, "quick");
//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ConflateStatsHelperTest, "current");

}

