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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/cmd/ConflateCmd.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/conflate/polygon/BuildingMatchCreator.h>
#include <hoot/core/conflate/highway/HighwayMatchCreator.h>
#include <hoot/core/conflate/poi-polygon/PoiPolygonMatchCreator.h>
#include <hoot/core/conflate/network/NetworkMatchCreator.h>
#include <hoot/core/conflate/matching/MatchFactory.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

namespace hoot
{

class ConflateCmdOpFilteringTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ConflateCmdOpFilteringTest);
  CPPUNIT_TEST(runOpFilterTest);
  CPPUNIT_TEST_SUITE_END();

public:

  ConflateCmdOpFilteringTest()
  {
    setResetType(ResetAll);
  }

  void runOpFilterTest()
  {
    QStringList matchCreators;
    QStringList actualOps;

    ConflateCmd uut;
    uut.setFilterOps(true);

    CPPUNIT_ASSERT_EQUAL(4,  TestUtils::getConflateCmdSnapshotPreOps().size());
    CPPUNIT_ASSERT_EQUAL(15,  TestUtils::getConflateCmdSnapshotPostOps().size());
    CPPUNIT_ASSERT_EQUAL(17,  TestUtils::getConflateCmdSnapshotCleaningOps().size());

    // test 1

    matchCreators.clear();
    matchCreators.append(QString::fromStdString(BuildingMatchCreator::className()));
    MatchFactory::getInstance().reset();
    MatchFactory::_setMatchCreators(matchCreators);
    // This is a snapshot of the ops in order to avoid any changes made to them result in requiring
    // this test's results to change over time. Clearly, any newly added ops could be being filtered
    // incorrectly, and we can update this list periodically if that's deemed important.
    conf().set(ConfigOptions::getConflatePreOpsKey(), TestUtils::getConflateCmdSnapshotPreOps());
    conf().set(ConfigOptions::getConflatePostOpsKey(), TestUtils::getConflateCmdSnapshotPostOps());
    conf().set(
      ConfigOptions::getMapCleanerTransformsKey(), TestUtils::getConflateCmdSnapshotCleaningOps());

    uut._removeSuperfluousOps();

    actualOps = conf().getList(ConfigOptions::getConflatePreOpsKey());
    CPPUNIT_ASSERT_EQUAL(2, actualOps.size());

    actualOps = conf().getList(ConfigOptions::getConflatePostOpsKey());
    CPPUNIT_ASSERT_EQUAL(13, actualOps.size());

    actualOps = conf().getList(ConfigOptions::getMapCleanerTransformsKey());
    CPPUNIT_ASSERT_EQUAL(11, actualOps.size());

    // test 2

    matchCreators.clear();
    matchCreators.append(QString::fromStdString(HighwayMatchCreator::className()));
    MatchFactory::getInstance().reset();
    MatchFactory::_setMatchCreators(matchCreators);
    conf().set(ConfigOptions::getConflatePreOpsKey(), TestUtils::getConflateCmdSnapshotPreOps());
    conf().set(ConfigOptions::getConflatePostOpsKey(), TestUtils::getConflateCmdSnapshotPostOps());
    conf().set(
      ConfigOptions::getMapCleanerTransformsKey(), TestUtils::getConflateCmdSnapshotCleaningOps());

    uut._removeSuperfluousOps();

    actualOps = conf().getList(ConfigOptions::getConflatePreOpsKey());
    CPPUNIT_ASSERT_EQUAL(3, actualOps.size());

    actualOps = conf().getList(ConfigOptions::getConflatePostOpsKey());
    CPPUNIT_ASSERT_EQUAL(14, actualOps.size());

    actualOps = conf().getList(ConfigOptions::getMapCleanerTransformsKey());
    CPPUNIT_ASSERT_EQUAL(15, actualOps.size());

    // test 3

    matchCreators.clear();
    matchCreators.append(QString::fromStdString(NetworkMatchCreator::className()));
    MatchFactory::getInstance().reset();
    MatchFactory::_setMatchCreators(matchCreators);
    conf().set(ConfigOptions::getConflatePreOpsKey(), TestUtils::getConflateCmdSnapshotPreOps());
    conf().set(ConfigOptions::getConflatePostOpsKey(), TestUtils::getConflateCmdSnapshotPostOps());
    conf().set(
      ConfigOptions::getMapCleanerTransformsKey(), TestUtils::getConflateCmdSnapshotCleaningOps());

    uut._removeSuperfluousOps();

    actualOps = conf().getList(ConfigOptions::getConflatePreOpsKey());
    CPPUNIT_ASSERT_EQUAL(3, actualOps.size());

    actualOps = conf().getList(ConfigOptions::getConflatePostOpsKey());
    CPPUNIT_ASSERT_EQUAL(14, actualOps.size());

    actualOps = conf().getList(ConfigOptions::getMapCleanerTransformsKey());
    CPPUNIT_ASSERT_EQUAL(15, actualOps.size());

    // test 4

    matchCreators.clear();
    matchCreators.append(QString::fromStdString(PoiPolygonMatchCreator::className()));
    MatchFactory::getInstance().reset();
    MatchFactory::_setMatchCreators(matchCreators);
    conf().set(ConfigOptions::getConflatePreOpsKey(), TestUtils::getConflateCmdSnapshotPreOps());
    conf().set(ConfigOptions::getConflatePostOpsKey(), TestUtils::getConflateCmdSnapshotPostOps());
    conf().set(
      ConfigOptions::getMapCleanerTransformsKey(), TestUtils::getConflateCmdSnapshotCleaningOps());

    uut._removeSuperfluousOps();

    actualOps = conf().getList(ConfigOptions::getConflatePreOpsKey());
    CPPUNIT_ASSERT_EQUAL(2, actualOps.size());

    actualOps = conf().getList(ConfigOptions::getConflatePostOpsKey());
    CPPUNIT_ASSERT_EQUAL(13, actualOps.size());

    actualOps = conf().getList(ConfigOptions::getMapCleanerTransformsKey());
    CPPUNIT_ASSERT_EQUAL(11, actualOps.size());

    // combined test

    matchCreators.clear();
    matchCreators.append(QString::fromStdString(BuildingMatchCreator::className()));
    matchCreators.append(QString::fromStdString(HighwayMatchCreator::className()));
    matchCreators.append(QString::fromStdString(NetworkMatchCreator::className()));
    matchCreators.append(QString::fromStdString(PoiPolygonMatchCreator::className()));
    MatchFactory::getInstance().reset();
    MatchFactory::_setMatchCreators(matchCreators);
    conf().set(ConfigOptions::getConflatePreOpsKey(), TestUtils::getConflateCmdSnapshotPreOps());
    conf().set(ConfigOptions::getConflatePostOpsKey(), TestUtils::getConflateCmdSnapshotPostOps());
    conf().set(
      ConfigOptions::getMapCleanerTransformsKey(), TestUtils::getConflateCmdSnapshotCleaningOps());

    uut._removeSuperfluousOps();

    actualOps = conf().getList(ConfigOptions::getConflatePreOpsKey());
    CPPUNIT_ASSERT_EQUAL(4, actualOps.size());

    actualOps = conf().getList(ConfigOptions::getConflatePostOpsKey());
    CPPUNIT_ASSERT_EQUAL(15, actualOps.size());

    actualOps = conf().getList(ConfigOptions::getMapCleanerTransformsKey());
    CPPUNIT_ASSERT_EQUAL(17, actualOps.size());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ConflateCmdOpFilteringTest, "quick");

}
