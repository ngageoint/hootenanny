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
#include <hoot/core/conflate/matching/OptionsValidator.h>
#include <hoot/core/util/Settings.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

namespace hoot
{

/*
 * Parts of this test should eventually be removed including:
 *
 * runAutoCorrectSublineMatcherTest
 * runAutoCorrectClassifierTest
 *
 * See notes in OptionsValidator.
 *
 * Not adding a test for empty matcher inputs, which resets them to defaults, as the output would
 * change every time additions are made to the matchers.
 */
class OptionsValidatorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(OptionsValidatorTest);
  CPPUNIT_TEST(runValidateSizeUnequalTest);
  CPPUNIT_TEST(runValidateScriptMismatchTest);
  CPPUNIT_TEST(runAutoCorrectGenericTest);
  CPPUNIT_TEST(runAutoCorrectCollectionRelationTest);
  CPPUNIT_TEST(runAutoCorrectMatcherOrderingTest);
  CPPUNIT_TEST(runAutoCorrectSublineMatcherTest);
  CPPUNIT_TEST(runAutoCorrectClassifierTest);
  CPPUNIT_TEST_SUITE_END();

public:

  OptionsValidatorTest()
  {
    setResetType(ResetAll);
  }

  void runValidateSizeUnequalTest()
  {
    QStringList matchers;
    matchers.append("hoot::BuildingMatchCreator");
    matchers.append("hoot::HighwayMatchCreator");
    conf().set("match.creators", matchers);
    QStringList mergers;
    mergers.append("hoot::BuildingMergerCreator");
    conf().set("merger.creators", mergers);

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
      "The number of configured match creators (2) does not equal the number of configured " +
      "merger creators (1)";
    HOOT_STR_EQUALS(expected.toStdString(), exceptionMsg.toStdString());
  }

  void runValidateScriptMismatchTest()
  {
    QStringList matchers;
    matchers.append("hoot::BuildingMatchCreator");
    matchers.append("hoot::ScriptMatchCreator,Area.js");
    conf().set("match.creators", matchers);
    QStringList mergers;
    mergers.append("hoot::BuildingMergerCreator");
    mergers.append("hoot::PoiPolygonMergerCreator");
    conf().set("merger.creators", mergers);

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
    conf().set("autocorrect.options", "true");

    QStringList matchers;
    matchers.append("hoot::ScriptMatchCreator,Point.js");
    matchers.append("hoot::ScriptMatchCreator,Area.js");
    matchers.append("hoot::BuildingMatchCreator");
    matchers.append("hoot::ScriptMatchCreator,Line.js");
    conf().set("match.creators", matchers);
    QStringList mergers;
    mergers.append("hoot::ScriptMergerCreator");
    mergers.append("hoot::ScriptMergerCreator");
    mergers.append("hoot::BuildingMergerCreator");
    mergers.append("hoot::ScriptMergerCreator");
    conf().set("merger.creators", mergers);

    OptionsValidator::fixGenericMatcherOrdering();

    matchers = conf().get("match.creators");
    mergers = conf().get("merger.creators");
    CPPUNIT_ASSERT_EQUAL(0, matchers.indexOf("hoot::ScriptMatchCreator,Area.js"));
    CPPUNIT_ASSERT_EQUAL(1, matchers.indexOf("hoot::BuildingMatchCreator"));
    CPPUNIT_ASSERT_EQUAL(2, matchers.indexOf("hoot::ScriptMatchCreator,Point.js"));
    CPPUNIT_ASSERT_EQUAL(3, matchers.indexOf("hoot::ScriptMatchCreator,Line.js"));
    CPPUNIT_ASSERT_EQUAL(0, mergers.indexOf("hoot::ScriptMergerCreator"));
    CPPUNIT_ASSERT_EQUAL(1, mergers.indexOf("hoot::BuildingMergerCreator"));
    CPPUNIT_ASSERT_EQUAL(2, mergers.indexOf("hoot::ScriptMergerCreator"));
    CPPUNIT_ASSERT_EQUAL(3, mergers.indexOf("hoot::ScriptMergerCreator"));
  }

  void runAutoCorrectCollectionRelationTest()
  {
    conf().set("autocorrect.options", "true");

    QStringList matchers;
    matchers.append("hoot::ScriptMatchCreator,Area.js");
    matchers.append("hoot::ScriptMatchCreator,CollectionRelations.js");
    matchers.append("hoot::BuildingMatchCreator");
    conf().set("match.creators", matchers);
    QStringList mergers;
    mergers.append("hoot::ScriptMergerCreator");
    mergers.append("hoot::ScriptMergerCreator");
    mergers.append("hoot::BuildingMergerCreator");
    conf().set("merger.creators", mergers);

    OptionsValidator::fixGenericMatcherOrdering();

    matchers = conf().get("match.creators");
    mergers = conf().get("merger.creators");
    CPPUNIT_ASSERT_EQUAL(0, matchers.indexOf("hoot::ScriptMatchCreator,Area.js"));
    CPPUNIT_ASSERT_EQUAL(1, matchers.indexOf("hoot::BuildingMatchCreator"));
    CPPUNIT_ASSERT_EQUAL(2, matchers.indexOf("hoot::ScriptMatchCreator,CollectionRelations.js"));
    CPPUNIT_ASSERT_EQUAL(0, mergers.indexOf("hoot::ScriptMergerCreator"));
    CPPUNIT_ASSERT_EQUAL(1, mergers.indexOf("hoot::BuildingMergerCreator"));
    CPPUNIT_ASSERT_EQUAL(2, mergers.indexOf("hoot::ScriptMergerCreator"));
  }

  void runAutoCorrectMatcherOrderingTest()
  {
    conf().set("autocorrect.options", "true");

    QStringList matchers;
    matchers.append("hoot::ScriptMatchCreator,Area.js");
    matchers.append("hoot::HighwayMatchCreator");
    matchers.append("hoot::BuildingMatchCreator");
    conf().set("match.creators", matchers);
    QStringList mergers;
    mergers.append("hoot::BuildingMergerCreator");
    mergers.append("hoot::HighwayMergerCreator");
    mergers.append("hoot::BuildingMergerCreator");
    conf().set("merger.creators", mergers);

    OptionsValidator::fixMisc();

    matchers = conf().get("match.creators");
    mergers = conf().get("merger.creators");
    CPPUNIT_ASSERT_EQUAL(0, matchers.indexOf("hoot::ScriptMatchCreator,Area.js"));
    CPPUNIT_ASSERT_EQUAL(1, matchers.indexOf("hoot::HighwayMatchCreator"));
    CPPUNIT_ASSERT_EQUAL(2, matchers.indexOf("hoot::BuildingMatchCreator"));
    CPPUNIT_ASSERT_EQUAL(0, mergers.indexOf("hoot::ScriptMergerCreator"));
    CPPUNIT_ASSERT_EQUAL(1, mergers.indexOf("hoot::HighwayMergerCreator"));
    CPPUNIT_ASSERT_EQUAL(2, mergers.indexOf("hoot::BuildingMergerCreator"));
  }

  void runAutoCorrectSublineMatcherTest()
  {
    conf().set("autocorrect.options", "true");

    QStringList matchers;
    matchers.append("hoot::NetworkMatchCreator");
    QStringList mergers;
    mergers.append("hoot::NetworkMergerCreator");
    conf().set("way.subline.matcher", "hoot::FrechetSublineMatcher");
    OptionsValidator::fixMisc();
    HOOT_STR_EQUALS("hoot::FrechetSublineMatcher", conf().get("way.subline.matcher"));

    TestUtils::resetEnvironment();
    matchers.clear();
    matchers.append("hoot::NetworkMatchCreator");
    mergers.clear();
    mergers.append("hoot::NetworkMergerCreator");
    conf().set("way.subline.matcher", "hoot::MaximalNearestSublineMatcher");
    OptionsValidator::fixMisc();
    HOOT_STR_EQUALS("hoot::MaximalNearestSublineMatcher", conf().get("way.subline.matcher"));

    TestUtils::resetEnvironment();
    matchers.clear();
    matchers.append("hoot::NetworkMatchCreator");
    mergers.clear();
    mergers.append("hoot::NetworkMergerCreator");
    conf().set("way.subline.matcher", "blah");
    OptionsValidator::fixMisc();
    HOOT_STR_EQUALS("hoot::MaximalNearestSublineMatcher", conf().get("way.subline.matcher"));

    matchers.clear();
    matchers.append("hoot::HighwayMatchCreator");
    mergers.clear();
    mergers.append("hoot::HighwayMergerCreator");
    conf().set("way.subline.matcher", "hoot::FrechetSublineMatcher");
    OptionsValidator::fixMisc();
    HOOT_STR_EQUALS("hoot::FrechetSublineMatcher", conf().get("way.subline.matcher"));

    TestUtils::resetEnvironment();
    matchers.clear();
    matchers.append("hoot::HighwayMatchCreator");
    mergers.clear();
    mergers.append("hoot::HighwayMergerCreator");
    conf().set("way.subline.matcher", "hoot::MaximalNearestSublineMatcher");
    OptionsValidator::fixMisc();
    HOOT_STR_EQUALS("hoot::MaximalNearestSublineMatcher", conf().get("way.subline.matcher"));

    TestUtils::resetEnvironment();
    matchers.clear();
    matchers.append("hoot::HighwayMatchCreator");
    mergers.clear();
    mergers.append("hoot:HighwayMergerCreator");
    conf().set("way.subline.matcher", "blah");
    OptionsValidator::fixMisc();
    HOOT_STR_EQUALS("hoot::MaximalNearestSublineMatcher", conf().get("way.subline.matcher"));
  }

  void runAutoCorrectClassifierTest()
  {
    conf().set("autocorrect.options", "true");

    QStringList matchers;
    matchers.append("hoot::NetworkMatchCreator");
    QStringList mergers;
    mergers.append("hoot::NetworkMergerCreator");
    conf().set("conflate.match.highway.classifier", "hoot::HighwayExpertClassifier");
    OptionsValidator::fixMisc();
    HOOT_STR_EQUALS(
      "hoot::HighwayExpertClassifier", conf().get("conflate.match.highway.classifier"));

    matchers.clear();
    matchers.append("hoot::NetworkMatchCreator");
    mergers.clear();
    mergers.append("hoot::NetworkMergerCreator");
    conf().set("conflate.match.highway.classifier", "hoot::HighwayRfClassifier");
    OptionsValidator::fixMisc();
    HOOT_STR_EQUALS(
      "hoot::HighwayExpertClassifier", conf().get("conflate.match.highway.classifier"));

    matchers.clear();
    matchers.append("hoot::HighwayMatchCreator");
    mergers.clear();
    mergers.append("hoot::HighwayMergerCreator");
    conf().set("conflate.match.highway.classifier", "hoot::HighwayRfClassifier");
    OptionsValidator::fixMisc();
    HOOT_STR_EQUALS(
      "hoot::HighwayRfClassifier", conf().get("conflate.match.highway.classifier"));

    matchers.clear();
    matchers.append("hoot::HighwayMatchCreator");
    mergers.clear();
    mergers.append("hoot::HighwayMergerCreator");
    conf().set("conflate.match.highway.classifier", "hoot::HighwayRfClassifier");
    OptionsValidator::fixMisc();
    HOOT_STR_EQUALS(
      "hoot::HighwayRfClassifier", conf().get("conflate.match.highway.classifier"));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OptionsValidatorTest, "quick");

}

