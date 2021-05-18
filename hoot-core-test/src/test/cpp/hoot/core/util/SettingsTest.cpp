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
 * @copyright Copyright (C) 2012, 2013, 2015, 2016, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/ops/ReplaceElementOp.h>
#include <hoot/core/visitors/RemoveElementsVisitor.h>
#include <hoot/core/criterion/BuildingCriterion.h>
#include <hoot/core/elements/Node.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

namespace hoot
{

class SettingsTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(SettingsTest);
  CPPUNIT_TEST(envTest);
  CPPUNIT_TEST(replaceTest);
  CPPUNIT_TEST(storeTest);
  CPPUNIT_TEST(baseSettingsTest);
  CPPUNIT_TEST(invalidOptionNameTest);
  CPPUNIT_TEST(invalidOperatorsTest);
  CPPUNIT_TEST_SUITE_END();

public:

  SettingsTest()
    : HootTestFixture(UNUSED_PATH,
                      "test-output/utils/")
  {
  }

  void envTest()
  {
    Settings uut;
    uut.loadEnvironment();
    char* path = getenv("PATH");
    HOOT_STR_EQUALS(QString(path), uut.getString("PATH"));

    uut.set("mypath", "my path: ${PATH}");
    HOOT_STR_EQUALS(QString("my path: ") + path, uut.getString("mypath"));
  }

  void replaceTest()
  {
    Settings uut;
    uut.loadDefaults();
    uut.set("perty.csm.D", "2");
    uut.set("perty.test.num.runs", 2);
    uut.set("map.factory.writer", "1");
    uut.set("map.factory.reader", "${perty.csm.D}");
    HOOT_STR_EQUALS(QString("2"), uut.getString("map.factory.reader"));

    uut.set("map.factory.reader", "${perty.csm.D} ${map.factory.writer}");
    HOOT_STR_EQUALS(QString("2 1"), uut.getString("map.factory.reader"));

    uut.set("map.factory.reader", "${map.factory.writer} ${map.factory.writer}");
    HOOT_STR_EQUALS(QString("1 1"), uut.getString("map.factory.reader"));

    uut.set("perty.csm.D", "${doesnt.exist}");
    HOOT_STR_EQUALS(QString(""), uut.getString("perty.csm.D"));

    HOOT_STR_EQUALS("", uut.getValue("${doesnt.exist}"));
    HOOT_STR_EQUALS("1", uut.getValue("${map.factory.writer}"));
    HOOT_STR_EQUALS(1, uut.getDoubleValue("${map.factory.writer}"));
    HOOT_STR_EQUALS(2, uut.getDoubleValue("${perty.test.num.runs}"));
  }

  void storeTest()
  {
    Settings uut;
    uut.loadDefaults();
    uut.set("perty.csm.D", "2");
    uut.set("map.factory.writer", "1");
    uut.set("map.factory.reader", "${perty.csm.D}");

    uut.storeJson(_outputPath + "SettingsTest.json");

    Settings uut2;
    uut2.loadDefaults();
    uut2.loadJson(_outputPath + "SettingsTest.json");
    HOOT_STR_EQUALS(uut.getString("perty.csm.D"), "2");
    HOOT_STR_EQUALS(uut.getString("map.factory.writer"), "1");
    HOOT_STR_EQUALS(uut.getString("map.factory.reader"), "2");

    Settings uut3;
    uut3.loadDefaults();
    uut3.loadFromString(uut.toString());
    HOOT_STR_EQUALS(uut.getString("perty.csm.D"), "2");
    HOOT_STR_EQUALS(uut.getString("map.factory.writer"), "1");
    HOOT_STR_EQUALS(uut.getString("map.factory.reader"), "2");
  }

  void baseSettingsTest()
  {
    Settings uut;
    uut.loadDefaults();

    //  The following
    //  Default value before change in JSON
    CPPUNIT_ASSERT_EQUAL(false, uut.getBool("uuid.helper.repeatable"));
    //  Default value before change in AttributeConflation.conf
    HOOT_STR_EQUALS("hoot::OverwriteTag2Merger", uut.getString("tag.merger.default"));
    HOOT_STR_EQUALS(
      "hoot::LinearSnapMerger", uut.getString("geometry.linear.merger.default"));
    //  Default value before change in NetworkAlgorithm.conf
    HOOT_STR_EQUALS("hoot::HighwayRfClassifier", uut.getString("conflate.match.highway.classifier"));
    HOOT_STR_EQUALS(
      "hoot::MaximalNearestSublineMatcher", uut.getString("way.subline.matcher"));

    uut.loadFromString("{ \"base.config\": \"AttributeConflation.conf,NetworkAlgorithm.conf\", \"uuid.helper.repeatable\": \"true\" }");
    //  From the JSON
    CPPUNIT_ASSERT_EQUAL(true, uut.getBool("uuid.helper.repeatable"));
    //  From AttributeConflation.conf
    HOOT_STR_EQUALS("hoot::OverwriteTag1Merger", uut.getString("tag.merger.default"));
    HOOT_STR_EQUALS(
      "hoot::LinearTagOnlyMerger", uut.getString("geometry.linear.merger.default"));
    //  From NetworkAlgorithm.conf
    HOOT_STR_EQUALS("hoot::HighwayExpertClassifier", uut.getString("conflate.match.highway.classifier"));
    HOOT_STR_EQUALS("hoot::MaximalSublineMatcher", uut.getString("way.subline.matcher"));
  }

  void invalidOptionNameTest()
  {
    QStringList args;
    args.append("-D");
    args.append("blah=true");
    QString exceptionMsg;
    try
    {
      Settings::parseCommonArguments(args);
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT_EQUAL(
      QString("Unknown settings option: (blah)").toStdString(), exceptionMsg.toStdString());
  }

  void invalidOperatorsTest()
  {
    QStringList args;
    QString exceptionMsg;
    QString expectedErrorMessage;

    args.append("-D");
    args.append(
      ConfigOptions::getConvertOpsKey() + "=" +
      ReplaceElementOp::className() + ";" +
      RemoveElementsVisitor::className() + ";" +
      BuildingCriterion::className() + ";" +
      // fails; only ops, vis, and crits are valid
      Node::className());
    try
    {
      Settings::parseCommonArguments(args);
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    expectedErrorMessage =
      "Invalid option operator class name: " + Node::className();
    CPPUNIT_ASSERT_EQUAL(expectedErrorMessage.toStdString(), exceptionMsg.toStdString());

    args.clear();
    args.append("-D");
    args.append(
      ConfigOptions::getConvertOpsKey() + "=" +
      ReplaceElementOp::className() + ";" +
      // fails; visitor is missing namespace
      RemoveElementsVisitor::className().replace("hoot::", ""));
    try
    {
      Settings::parseCommonArguments(args);
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    expectedErrorMessage =
      "Invalid option operator class name: " +
      RemoveElementsVisitor::className().replace("hoot::", "");
    CPPUNIT_ASSERT_EQUAL(expectedErrorMessage.toStdString(), exceptionMsg.toStdString());

    args.clear();
    args.append("-D");
    args.append(
      ConfigOptions::getConvertOpsKey() + "=" +
      ReplaceElementOp::className() + ";" +
      RemoveElementsVisitor::className() + ";" +
      "blah");
    exceptionMsg = "";
    try
    {
      Settings::parseCommonArguments(args);
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    expectedErrorMessage = "Invalid option operator class name: blah";
    CPPUNIT_ASSERT_EQUAL(expectedErrorMessage.toStdString(), exceptionMsg.toStdString());
  }
};

}

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(hoot::SettingsTest, "quick");

