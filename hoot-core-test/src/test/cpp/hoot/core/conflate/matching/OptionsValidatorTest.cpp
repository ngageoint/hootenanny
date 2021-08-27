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
 * @copyright Copyright (C) 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/subline-matching/FrechetSublineMatcher.h>
#include <hoot/core/algorithms/subline-matching/MaximalNearestSublineMatcher.h>
#include <hoot/core/algorithms/subline-matching/MaximalSublineMatcher.h>
#include <hoot/core/conflate/highway/HighwayExpertClassifier.h>
#include <hoot/core/conflate/highway/HighwayRfClassifier.h>
#include <hoot/core/conflate/highway/HighwayMatchCreator.h>
#include <hoot/core/conflate/highway/HighwayMergerCreator.h>
#include <hoot/core/conflate/matching/OptionsValidator.h>
#include <hoot/core/conflate/network/NetworkMatchCreator.h>
#include <hoot/core/conflate/network/NetworkMergerCreator.h>
#include <hoot/core/conflate/poi-polygon/PoiPolygonMergerCreator.h>
#include <hoot/core/conflate/polygon/BuildingMatchCreator.h>
#include <hoot/core/conflate/polygon/BuildingMergerCreator.h>
#include <hoot/core/util/Settings.h>

namespace hoot
{

/*
 * Parts of this test should eventually be removed including:
 *
 * runAutoCorrectSublineMatcherTest
 * runAutoCorrectClassifierTest
 *
 * See notes in OptionsValidator.
 */
class OptionsValidatorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(OptionsValidatorTest);
  CPPUNIT_TEST(runValidateSizeUnequalTest);
  CPPUNIT_TEST(runValidateScriptMismatchTest);
  CPPUNIT_TEST(runAutoCorrectGenericTest);
  CPPUNIT_TEST(runAutoCorrectRelationScriptTest);
  CPPUNIT_TEST(runAutoCorrectMatcherOrderingTest);
  CPPUNIT_TEST(runAutoCorrectSublineMatcherTest);
  CPPUNIT_TEST(runAutoCorrectClassifierTest);
  CPPUNIT_TEST_SUITE_END();

public:

  OptionsValidatorTest()
  {
    setResetType(ResetAllNoMatchFactory);
  }

  void runValidateSizeUnequalTest()
  {
    // Not adding a test for empty matcher inputs, which resets them to defaults, as the output
    // would change every time additions are made to the matchers.

    QStringList matchers;
    matchers.append(BuildingMatchCreator::className());
    matchers.append(HighwayMatchCreator::className());
    conf().set(ConfigOptions::getMatchCreatorsKey(), matchers);
    QStringList mergers;
    mergers.append(BuildingMergerCreator::className());
    conf().set(ConfigOptions::getMergerCreatorsKey(), mergers);

    QString exceptionMsg;
    try
    {
      OptionsValidator::validateMatchers();
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    const QString expected =
      QString("The number of configured match creators (2) does not equal the number of ") +
      QString("configured merger creators (1)");
    HOOT_STR_EQUALS(expected.toStdString(), exceptionMsg.toStdString());
  }

  void runValidateScriptMismatchTest()
  {
    QStringList matchers;
    matchers.append(BuildingMatchCreator::className());
    matchers.append("ScriptMatchCreator,Area.js");
    conf().set(ConfigOptions::getMatchCreatorsKey(), matchers);
    QStringList mergers;
    mergers.append(BuildingMergerCreator::className());
    mergers.append(PoiPolygonMergerCreator::className());
    conf().set(ConfigOptions::getMergerCreatorsKey(), mergers);

    QString exceptionMsg;
    try
    {
      OptionsValidator::validateMatchers();
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    const QString expected =
      "Attempted to use a ScriptMatchCreator without a ScriptMergerCreator";
    CPPUNIT_ASSERT(expected.startsWith(expected));
  }

  void runAutoCorrectGenericTest()
  {
    conf().set(ConfigOptions::getAutocorrectOptionsKey(), "true");

    QStringList matchers;
    matchers.append("ScriptMatchCreator,Point.js");
    matchers.append("ScriptMatchCreator,Area.js");
    matchers.append(BuildingMatchCreator::className());
    matchers.append("ScriptMatchCreator,Line.js");
    conf().set(ConfigOptions::getMatchCreatorsKey(), matchers);
    QStringList mergers;
    mergers.append("ScriptMergerCreator");
    mergers.append("ScriptMergerCreator");
    mergers.append(BuildingMergerCreator::className());
    mergers.append("ScriptMergerCreator");
    conf().set(ConfigOptions::getMergerCreatorsKey(), mergers);

    OptionsValidator::fixGenericMatcherOrdering();

    matchers = conf().getList(ConfigOptions::getMatchCreatorsKey());
    LOG_VART(matchers);
    mergers = conf().getList(ConfigOptions::getMergerCreatorsKey());
    LOG_VART(mergers);
    CPPUNIT_ASSERT_EQUAL(0, matchers.indexOf("ScriptMatchCreator,Area.js"));
    CPPUNIT_ASSERT_EQUAL(1, matchers.indexOf(BuildingMatchCreator::className()));
    CPPUNIT_ASSERT_EQUAL(2, matchers.indexOf("ScriptMatchCreator,Point.js"));
    CPPUNIT_ASSERT_EQUAL(3, matchers.indexOf("ScriptMatchCreator,Line.js"));
    HOOT_STR_EQUALS("ScriptMergerCreator", mergers.at(0).toStdString());
    CPPUNIT_ASSERT_EQUAL(1, mergers.indexOf(BuildingMergerCreator::className()));
    HOOT_STR_EQUALS("ScriptMergerCreator", mergers.at(2).toStdString());
    HOOT_STR_EQUALS("ScriptMergerCreator", mergers.at(3).toStdString());
  }

  void runAutoCorrectRelationScriptTest()
  {
    conf().set(ConfigOptions::getAutocorrectOptionsKey(), "true");

    QStringList matchers;
    matchers.append("ScriptMatchCreator,Area.js");
    matchers.append("ScriptMatchCreator,Relation.js");
    matchers.append(BuildingMatchCreator::className());
    conf().set(ConfigOptions::getMatchCreatorsKey(), matchers);
    QStringList mergers;
    mergers.append("ScriptMergerCreator");
    mergers.append("ScriptMergerCreator");
    mergers.append(BuildingMergerCreator::className());
    conf().set(ConfigOptions::getMergerCreatorsKey(), mergers);

    OptionsValidator::fixGenericMatcherOrdering();

    matchers = conf().getList(ConfigOptions::getMatchCreatorsKey());
    mergers = conf().getList(ConfigOptions::getMergerCreatorsKey());
    LOG_VART(matchers);
    LOG_VART(mergers);
    CPPUNIT_ASSERT_EQUAL(0, matchers.indexOf("ScriptMatchCreator,Area.js"));
    CPPUNIT_ASSERT_EQUAL(1, matchers.indexOf(BuildingMatchCreator::className()));
    CPPUNIT_ASSERT_EQUAL(2, matchers.indexOf("ScriptMatchCreator,Relation.js"));
    HOOT_STR_EQUALS("ScriptMergerCreator", mergers.at(0).toStdString());
    CPPUNIT_ASSERT_EQUAL(1, mergers.indexOf(BuildingMergerCreator::className()));
    HOOT_STR_EQUALS("ScriptMergerCreator", mergers.at(2).toStdString());
  }

  void runAutoCorrectMatcherOrderingTest()
  {
    conf().set(ConfigOptions::getAutocorrectOptionsKey(), "true");

    QStringList matchers;
    matchers.append("ScriptMatchCreator,Area.js");
    matchers.append(HighwayMatchCreator::className());
    matchers.append(BuildingMatchCreator::className());
    conf().set(ConfigOptions::getMatchCreatorsKey(), matchers);
    QStringList mergers;
    mergers.append(BuildingMergerCreator::className());
    mergers.append(HighwayMergerCreator::className());
    conf().set(ConfigOptions::getMergerCreatorsKey(), mergers);

    OptionsValidator::fixMisc();

    matchers = conf().getList(ConfigOptions::getMatchCreatorsKey());
    mergers = conf().getList(ConfigOptions::getMergerCreatorsKey());
    LOG_VART(matchers);
    LOG_VART(mergers);
    CPPUNIT_ASSERT_EQUAL(0, matchers.indexOf("ScriptMatchCreator,Area.js"));
    CPPUNIT_ASSERT_EQUAL(1, matchers.indexOf(HighwayMatchCreator::className()));
    CPPUNIT_ASSERT_EQUAL(2, matchers.indexOf(BuildingMatchCreator::className()));
    HOOT_STR_EQUALS("ScriptMergerCreator", mergers.at(0).toStdString());
    CPPUNIT_ASSERT_EQUAL(1, mergers.indexOf(HighwayMergerCreator::className()));
    CPPUNIT_ASSERT_EQUAL(2, mergers.indexOf(BuildingMergerCreator::className()));
  }

  void runAutoCorrectSublineMatcherTest()
  {
    conf().set(ConfigOptions::getAutocorrectOptionsKey(), "true");

    QStringList matchers;
    matchers.append(NetworkMatchCreator::className());
    conf().set(ConfigOptions::getMatchCreatorsKey(), matchers.join(";"));
    QStringList mergers;
    mergers.append(NetworkMergerCreator::className());
    conf().set(ConfigOptions::getMergerCreatorsKey(), mergers.join(";"));
    conf().set(ConfigOptions::getHighwaySublineMatcherKey(), FrechetSublineMatcher::className());
    OptionsValidator::fixMisc();
    HOOT_STR_EQUALS(
      FrechetSublineMatcher::className(), conf().get(ConfigOptions::getHighwaySublineMatcherKey()));

    TestUtils::resetEnvironment();
    matchers.clear();
    matchers.append(NetworkMatchCreator::className());
    conf().set(ConfigOptions::getMatchCreatorsKey(), matchers.join(";"));
    mergers.clear();
    mergers.append(NetworkMergerCreator::className());
    conf().set(ConfigOptions::getMergerCreatorsKey(), mergers.join(";"));
    conf().set(ConfigOptions::getHighwaySublineMatcherKey(), MaximalSublineMatcher::className());
    OptionsValidator::fixMisc();
    HOOT_STR_EQUALS(
      MaximalSublineMatcher::className(), conf().get(ConfigOptions::getHighwaySublineMatcherKey()));

    TestUtils::resetEnvironment();
    matchers.clear();
    matchers.append(NetworkMatchCreator::className());
    conf().set(ConfigOptions::getMatchCreatorsKey(), matchers.join(";"));
    mergers.clear();
    mergers.append(NetworkMergerCreator::className());
    conf().set(ConfigOptions::getMergerCreatorsKey(), mergers.join(";"));
    conf().set(ConfigOptions::getHighwaySublineMatcherKey(), "blah");
    OptionsValidator::fixMisc();
    HOOT_STR_EQUALS(
      MaximalSublineMatcher::className(), conf().get(ConfigOptions::getHighwaySublineMatcherKey()));

    matchers.clear();
    matchers.append(HighwayMatchCreator::className());
    conf().set(ConfigOptions::getMatchCreatorsKey(), matchers.join(";"));
    mergers.clear();
    mergers.append(HighwayMergerCreator::className());
    conf().set(ConfigOptions::getMergerCreatorsKey(), mergers.join(";"));
    conf().set(ConfigOptions::getHighwaySublineMatcherKey(), FrechetSublineMatcher::className());
    OptionsValidator::fixMisc();
    HOOT_STR_EQUALS(
      FrechetSublineMatcher::className(), conf().get(ConfigOptions::getHighwaySublineMatcherKey()));

    TestUtils::resetEnvironment();
    matchers.clear();
    matchers.append(HighwayMatchCreator::className());
    conf().set(ConfigOptions::getMatchCreatorsKey(), matchers.join(";"));
    mergers.clear();
    mergers.append(HighwayMergerCreator::className());
    conf().set(ConfigOptions::getMergerCreatorsKey(), mergers.join(";"));
    conf().set(
      ConfigOptions::getHighwaySublineMatcherKey(), MaximalNearestSublineMatcher::className());
    OptionsValidator::fixMisc();
    HOOT_STR_EQUALS(
      MaximalNearestSublineMatcher::className(),
      conf().get(ConfigOptions::getHighwaySublineMatcherKey()));

    TestUtils::resetEnvironment();
    matchers.clear();
    matchers.append(HighwayMatchCreator::className());
    conf().set(ConfigOptions::getMatchCreatorsKey(), matchers.join(";"));
    mergers.clear();
    mergers.append("hoot:" + HighwayMergerCreator::className());
    conf().set(ConfigOptions::getMergerCreatorsKey(), mergers.join(";"));
    conf().set(ConfigOptions::getHighwaySublineMatcherKey(), "blah");
    OptionsValidator::fixMisc();
    HOOT_STR_EQUALS(
      MaximalNearestSublineMatcher::className(),
      conf().get(ConfigOptions::getHighwaySublineMatcherKey()));
  }

  void runAutoCorrectClassifierTest()
  {
    conf().set(ConfigOptions::getAutocorrectOptionsKey(), "true");

    QStringList matchers;
    matchers.append(NetworkMatchCreator::className());
    conf().set(ConfigOptions::getMatchCreatorsKey(), matchers.join(";"));
    QStringList mergers;
    mergers.append(NetworkMergerCreator::className());
    conf().set(ConfigOptions::getMergerCreatorsKey(), mergers.join(";"));
    conf().set(
      ConfigOptions::getConflateMatchHighwayClassifierKey(), HighwayExpertClassifier::className());
    OptionsValidator::fixMisc();
    HOOT_STR_EQUALS(
      HighwayExpertClassifier::className(),
      conf().get(ConfigOptions::getConflateMatchHighwayClassifierKey()));

    matchers.clear();
    matchers.append(NetworkMatchCreator::className());
    conf().set(ConfigOptions::getMatchCreatorsKey(), matchers.join(";"));
    mergers.clear();
    mergers.append(NetworkMergerCreator::className());
    conf().set(ConfigOptions::getMergerCreatorsKey(), mergers.join(";"));
    conf().set(
      ConfigOptions::getConflateMatchHighwayClassifierKey(), HighwayRfClassifier::className());
    OptionsValidator::fixMisc();
    HOOT_STR_EQUALS(
      HighwayExpertClassifier::className(),
      conf().get(ConfigOptions::getConflateMatchHighwayClassifierKey()));

    matchers.clear();
    matchers.append(HighwayMatchCreator::className());
    conf().set(ConfigOptions::getMatchCreatorsKey(), matchers.join(";"));
    mergers.clear();
    mergers.append(HighwayMergerCreator::className());
    conf().set(ConfigOptions::getMergerCreatorsKey(), mergers.join(";"));
    conf().set(
      ConfigOptions::getConflateMatchHighwayClassifierKey(), HighwayRfClassifier::className());
    OptionsValidator::fixMisc();
    HOOT_STR_EQUALS(
      HighwayRfClassifier::className(),
      conf().get(ConfigOptions::getConflateMatchHighwayClassifierKey()));

    matchers.clear();
    matchers.append(HighwayMatchCreator::className());
    conf().set(ConfigOptions::getMatchCreatorsKey(), matchers.join(";"));
    mergers.clear();
    mergers.append(HighwayMergerCreator::className());
    conf().set(ConfigOptions::getMergerCreatorsKey(), mergers.join(";"));
    conf().set(
      ConfigOptions::getConflateMatchHighwayClassifierKey(), HighwayRfClassifier::className());
    OptionsValidator::fixMisc();
    HOOT_STR_EQUALS(
      HighwayRfClassifier::className(),
      conf().get(ConfigOptions::getConflateMatchHighwayClassifierKey()));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OptionsValidatorTest, "quick");

}

