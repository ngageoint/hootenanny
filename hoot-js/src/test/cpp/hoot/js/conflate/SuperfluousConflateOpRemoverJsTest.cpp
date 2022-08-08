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
 * @copyright Copyright (C) 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/js/conflate/matching/ScriptMatchCreator.h>

namespace hoot
{

class SuperfluousConflateOpRemoverJsTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(SuperfluousConflateOpRemoverJsTest);
  CPPUNIT_TEST(runOpFilter1Test);
  CPPUNIT_TEST(runOpFilter2Test);
  CPPUNIT_TEST(runOpFilter3Test);
  CPPUNIT_TEST(runOpFilter4Test);
  CPPUNIT_TEST(runOpFilter5Test);
  CPPUNIT_TEST(runOpFilter6Test);
  CPPUNIT_TEST(runOpFilter7Test);
  CPPUNIT_TEST(runOpFilter8Test);
  CPPUNIT_TEST(runOpFilter9Test);
  CPPUNIT_TEST(runOpFilterCombinedTest);
  CPPUNIT_TEST_SUITE_END();

public:

  SuperfluousConflateOpRemoverJsTest()
  {
    setResetType(ResetEnvironment);

    _scriptMatchCreatorName = ScriptMatchCreator::className();
  }

  void runOpFilter1Test()
  {
    TestUtils::runConflateOpReductionTest(
      QStringList(_scriptMatchCreatorName + ",Area.js"), 1, 12, 11);
  }

  void runOpFilter2Test()
  {
    TestUtils::runConflateOpReductionTest(
      QStringList(_scriptMatchCreatorName + ",Line.js"), 1, 12, 10);
  }

  void runOpFilter3Test()
  {
    TestUtils::runConflateOpReductionTest(
      QStringList(_scriptMatchCreatorName + ",Poi.js"), 1, 9, 7);
  }

  void runOpFilter4Test()
  {
    TestUtils::runConflateOpReductionTest(
      QStringList(_scriptMatchCreatorName + ",Point.js"), 1, 9, 7);
  }

  void runOpFilter5Test()
  {
    TestUtils::runConflateOpReductionTest(
      QStringList(_scriptMatchCreatorName + ",PointPolygon.js"), 1, 12, 9);
  }

  void runOpFilter6Test()
  {
    TestUtils::runConflateOpReductionTest(
      QStringList(_scriptMatchCreatorName + ",Polygon.js"), 1, 12, 9);
  }

  void runOpFilter7Test()
  {
    TestUtils::runConflateOpReductionTest(
      QStringList(_scriptMatchCreatorName + ",PowerLine.js"), 1, 12, 11);
  }

  void runOpFilter8Test()
  {
    TestUtils::runConflateOpReductionTest(
      QStringList(_scriptMatchCreatorName + ",Railway.js"), 1, 12, 11);
  }

  void runOpFilter9Test()
  {
    TestUtils::runConflateOpReductionTest(
      QStringList(_scriptMatchCreatorName + ",River.js"), 1, 12, 11);
  }

  void runOpFilterCombinedTest()
  {
    QStringList matchCreators;
    matchCreators.append(_scriptMatchCreatorName + ",Area.js");
    matchCreators.append(_scriptMatchCreatorName + ",Line.js");
    matchCreators.append(_scriptMatchCreatorName + ",Poi.js");
    matchCreators.append(_scriptMatchCreatorName + ",Point.js");
    matchCreators.append(_scriptMatchCreatorName + ",PointPolygon.js");
    matchCreators.append(_scriptMatchCreatorName + ",Polygon.js");
    matchCreators.append(_scriptMatchCreatorName + ",PowerLine.js");
    matchCreators.append(_scriptMatchCreatorName + ",Railway.js");
    matchCreators.append(_scriptMatchCreatorName + ",River.js");

    TestUtils::runConflateOpReductionTest(matchCreators, 1, 12, 13);
  }

private:

  QString _scriptMatchCreatorName;
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(SuperfluousConflateOpRemoverJsTest, "quick");

}
