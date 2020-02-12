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
#include <hoot/js/conflate/matching/ScriptMatchCreator.h>
#include <hoot/core/conflate/matching/MatchFactory.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

namespace hoot
{

class ConflateCmdOpFilteringJsTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ConflateCmdOpFilteringJsTest);
  CPPUNIT_TEST(runOpFilterTest);
  CPPUNIT_TEST_SUITE_END();

public:

  ConflateCmdOpFilteringJsTest()
  {
    setResetType(ResetAll);
  }

  void runOpFilterTest()
  {
    QStringList matchCreators;
    QStringList actualOps;
    const QString scriptMatchCreatorName = QString::fromStdString(ScriptMatchCreator::className());

    ConflateCmd uut;
    uut.setFilterOps(true);

    // test 1

    matchCreators.clear();
    matchCreators.append(scriptMatchCreatorName + ",Area.js");
    MatchFactory::getInstance().reset();
    MatchFactory::_setMatchCreators(matchCreators);
    // This is a snapshot of the ops to avoid changes to them requiring this test to change over
    // time.
    conf().set(ConfigOptions::getConflatePreOpsKey(), TestUtils::getConflateCmdSnapshotPreOps());
    conf().set(ConfigOptions::getConflatePostOpsKey(), TestUtils::getConflateCmdSnapshotPostOps());
    conf().set(
      ConfigOptions::getMapCleanerTransformsKey(), TestUtils::getConflateCmdSnapshotCleaningOps());

    uut._removeSuperfluousOps();

    actualOps = conf().getList(ConfigOptions::getConflatePreOpsKey());
    CPPUNIT_ASSERT_EQUAL(1, actualOps.size());

    actualOps = conf().getList(ConfigOptions::getConflatePostOpsKey());
    CPPUNIT_ASSERT_EQUAL(12, actualOps.size());

    actualOps = conf().getList(ConfigOptions::getMapCleanerTransformsKey());
    CPPUNIT_ASSERT_EQUAL(11, actualOps.size());

    // test 2

    matchCreators.clear();
    matchCreators.append(scriptMatchCreatorName + ",Line.js");
    MatchFactory::getInstance().reset();
    MatchFactory::_setMatchCreators(matchCreators);
    conf().set(ConfigOptions::getConflatePreOpsKey(), TestUtils::getConflateCmdSnapshotPreOps());
    conf().set(ConfigOptions::getConflatePostOpsKey(), TestUtils::getConflateCmdSnapshotPostOps());
    conf().set(
      ConfigOptions::getMapCleanerTransformsKey(), TestUtils::getConflateCmdSnapshotCleaningOps());

    uut._removeSuperfluousOps();

    actualOps = conf().getList(ConfigOptions::getConflatePreOpsKey());
    CPPUNIT_ASSERT_EQUAL(1, actualOps.size());

    actualOps = conf().getList(ConfigOptions::getConflatePostOpsKey());
    CPPUNIT_ASSERT_EQUAL(12, actualOps.size());

    actualOps = conf().getList(ConfigOptions::getMapCleanerTransformsKey());
    CPPUNIT_ASSERT_EQUAL(10, actualOps.size());

    // test 3

    matchCreators.clear();
    matchCreators.append(scriptMatchCreatorName + ",Poi.js");
    MatchFactory::getInstance().reset();
    MatchFactory::_setMatchCreators(matchCreators);
    conf().set(ConfigOptions::getConflatePreOpsKey(), TestUtils::getConflateCmdSnapshotPreOps());
    conf().set(ConfigOptions::getConflatePostOpsKey(), TestUtils::getConflateCmdSnapshotPostOps());
    conf().set(
      ConfigOptions::getMapCleanerTransformsKey(), TestUtils::getConflateCmdSnapshotCleaningOps());

    uut._removeSuperfluousOps();

    actualOps = conf().getList(ConfigOptions::getConflatePreOpsKey());
    CPPUNIT_ASSERT_EQUAL(1, actualOps.size());

    actualOps = conf().getList(ConfigOptions::getConflatePostOpsKey());
    CPPUNIT_ASSERT_EQUAL(9, actualOps.size());

    actualOps = conf().getList(ConfigOptions::getMapCleanerTransformsKey());
    CPPUNIT_ASSERT_EQUAL(7, actualOps.size());

    // test 4

    matchCreators.clear();
    matchCreators.append(scriptMatchCreatorName + ",Point.js");
    MatchFactory::getInstance().reset();
    MatchFactory::_setMatchCreators(matchCreators);
    conf().set(ConfigOptions::getConflatePreOpsKey(), TestUtils::getConflateCmdSnapshotPreOps());
    conf().set(ConfigOptions::getConflatePostOpsKey(), TestUtils::getConflateCmdSnapshotPostOps());
    conf().set(
      ConfigOptions::getMapCleanerTransformsKey(), TestUtils::getConflateCmdSnapshotCleaningOps());

    uut._removeSuperfluousOps();

    actualOps = conf().getList(ConfigOptions::getConflatePreOpsKey());
    CPPUNIT_ASSERT_EQUAL(1, actualOps.size());

    actualOps = conf().getList(ConfigOptions::getConflatePostOpsKey());
    CPPUNIT_ASSERT_EQUAL(9, actualOps.size());

    actualOps = conf().getList(ConfigOptions::getMapCleanerTransformsKey());
    CPPUNIT_ASSERT_EQUAL(7, actualOps.size());

    // test 5

    matchCreators.clear();
    matchCreators.append(scriptMatchCreatorName + ",PointPolygon.js");
    MatchFactory::getInstance().reset();
    MatchFactory::_setMatchCreators(matchCreators);
    conf().set(ConfigOptions::getConflatePreOpsKey(), TestUtils::getConflateCmdSnapshotPreOps());
    conf().set(ConfigOptions::getConflatePostOpsKey(), TestUtils::getConflateCmdSnapshotPostOps());
    conf().set(
      ConfigOptions::getMapCleanerTransformsKey(), TestUtils::getConflateCmdSnapshotCleaningOps());

    uut._removeSuperfluousOps();

    actualOps = conf().getList(ConfigOptions::getConflatePreOpsKey());
    CPPUNIT_ASSERT_EQUAL(1, actualOps.size());

    actualOps = conf().getList(ConfigOptions::getConflatePostOpsKey());
    CPPUNIT_ASSERT_EQUAL(12, actualOps.size());

    actualOps = conf().getList(ConfigOptions::getMapCleanerTransformsKey());
    CPPUNIT_ASSERT_EQUAL(9, actualOps.size());

    // test 6

    matchCreators.clear();
    matchCreators.append(scriptMatchCreatorName + ",Polygon.js");
    MatchFactory::getInstance().reset();
    MatchFactory::_setMatchCreators(matchCreators);
    conf().set(ConfigOptions::getConflatePreOpsKey(), TestUtils::getConflateCmdSnapshotPreOps());
    conf().set(ConfigOptions::getConflatePostOpsKey(), TestUtils::getConflateCmdSnapshotPostOps());
    conf().set(
      ConfigOptions::getMapCleanerTransformsKey(), TestUtils::getConflateCmdSnapshotCleaningOps());

    uut._removeSuperfluousOps();

    actualOps = conf().getList(ConfigOptions::getConflatePreOpsKey());
    CPPUNIT_ASSERT_EQUAL(1, actualOps.size());

    actualOps = conf().getList(ConfigOptions::getConflatePostOpsKey());
    CPPUNIT_ASSERT_EQUAL(12, actualOps.size());

    actualOps = conf().getList(ConfigOptions::getMapCleanerTransformsKey());
    CPPUNIT_ASSERT_EQUAL(9, actualOps.size());

    // test 7

    matchCreators.clear();
    matchCreators.append(scriptMatchCreatorName + ",PowerLine.js");
    MatchFactory::getInstance().reset();
    MatchFactory::_setMatchCreators(matchCreators);
    conf().set(ConfigOptions::getConflatePreOpsKey(), TestUtils::getConflateCmdSnapshotPreOps());
    conf().set(ConfigOptions::getConflatePostOpsKey(), TestUtils::getConflateCmdSnapshotPostOps());
    conf().set(
      ConfigOptions::getMapCleanerTransformsKey(), TestUtils::getConflateCmdSnapshotCleaningOps());

    uut._removeSuperfluousOps();

    actualOps = conf().getList(ConfigOptions::getConflatePreOpsKey());
    CPPUNIT_ASSERT_EQUAL(1, actualOps.size());

    actualOps = conf().getList(ConfigOptions::getConflatePostOpsKey());
    CPPUNIT_ASSERT_EQUAL(12, actualOps.size());

    actualOps = conf().getList(ConfigOptions::getMapCleanerTransformsKey());
    CPPUNIT_ASSERT_EQUAL(11, actualOps.size());

    // test 8

    matchCreators.clear();
    matchCreators.append(scriptMatchCreatorName + ",Railway.js");
    MatchFactory::getInstance().reset();
    MatchFactory::_setMatchCreators(matchCreators);
    conf().set(ConfigOptions::getConflatePreOpsKey(), TestUtils::getConflateCmdSnapshotPreOps());
    conf().set(ConfigOptions::getConflatePostOpsKey(), TestUtils::getConflateCmdSnapshotPostOps());
    conf().set(
      ConfigOptions::getMapCleanerTransformsKey(), TestUtils::getConflateCmdSnapshotCleaningOps());

    uut._removeSuperfluousOps();

    actualOps = conf().getList(ConfigOptions::getConflatePreOpsKey());
    CPPUNIT_ASSERT_EQUAL(1, actualOps.size());

    actualOps = conf().getList(ConfigOptions::getConflatePostOpsKey());
    CPPUNIT_ASSERT_EQUAL(12, actualOps.size());

    actualOps = conf().getList(ConfigOptions::getMapCleanerTransformsKey());
    CPPUNIT_ASSERT_EQUAL(11, actualOps.size());

    // test 9

    matchCreators.clear();
    matchCreators.append(scriptMatchCreatorName + ",River.js");
    MatchFactory::getInstance().reset();
    MatchFactory::_setMatchCreators(matchCreators);
    conf().set(ConfigOptions::getConflatePreOpsKey(), TestUtils::getConflateCmdSnapshotPreOps());
    conf().set(ConfigOptions::getConflatePostOpsKey(), TestUtils::getConflateCmdSnapshotPostOps());
    conf().set(
      ConfigOptions::getMapCleanerTransformsKey(), TestUtils::getConflateCmdSnapshotCleaningOps());

    uut._removeSuperfluousOps();

    actualOps = conf().getList(ConfigOptions::getConflatePreOpsKey());
    CPPUNIT_ASSERT_EQUAL(1, actualOps.size());

    actualOps = conf().getList(ConfigOptions::getConflatePostOpsKey());
    CPPUNIT_ASSERT_EQUAL(12, actualOps.size());

    actualOps = conf().getList(ConfigOptions::getMapCleanerTransformsKey());
    CPPUNIT_ASSERT_EQUAL(11, actualOps.size());

    // combined test

    matchCreators.clear();
    matchCreators.append(scriptMatchCreatorName + ",Area.js");
    matchCreators.append(scriptMatchCreatorName + ",Line.js");
    matchCreators.append(scriptMatchCreatorName + ",Poi.js");
    matchCreators.append(scriptMatchCreatorName + ",Point.js");
    matchCreators.append(scriptMatchCreatorName + ",PointPolygon.js");
    matchCreators.append(scriptMatchCreatorName + ",Polygon.js");
    matchCreators.append(scriptMatchCreatorName + ",PowerLine.js");
    matchCreators.append(scriptMatchCreatorName + ",Railway.js");
    matchCreators.append(scriptMatchCreatorName + ",River.js");
    MatchFactory::getInstance().reset();
    MatchFactory::_setMatchCreators(matchCreators);
    conf().set(ConfigOptions::getConflatePreOpsKey(), TestUtils::getConflateCmdSnapshotPreOps());
    conf().set(ConfigOptions::getConflatePostOpsKey(), TestUtils::getConflateCmdSnapshotPostOps());
    conf().set(
      ConfigOptions::getMapCleanerTransformsKey(), TestUtils::getConflateCmdSnapshotCleaningOps());

    uut._removeSuperfluousOps();

    actualOps = conf().getList(ConfigOptions::getConflatePreOpsKey());
    CPPUNIT_ASSERT_EQUAL(1, actualOps.size());

    actualOps = conf().getList(ConfigOptions::getConflatePostOpsKey());
    CPPUNIT_ASSERT_EQUAL(12, actualOps.size());

    actualOps = conf().getList(ConfigOptions::getMapCleanerTransformsKey());
    CPPUNIT_ASSERT_EQUAL(13, actualOps.size());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ConflateCmdOpFilteringJsTest, "quick");

}
