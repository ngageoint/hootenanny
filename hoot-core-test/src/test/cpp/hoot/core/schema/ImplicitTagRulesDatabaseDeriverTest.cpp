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
 * @copyright Copyright (C) 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/ImplicitTagRulesSqliteReader.h>
#include <hoot/core/schema/ImplicitTagRulesDatabaseDeriver.h>

namespace hoot
{

class ImplicitTagRulesDatabaseDeriverTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ImplicitTagRulesDatabaseDeriverTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST(runMinTagOccurrencePerWordTest);
  CPPUNIT_TEST(runMinWordLengthTest);
  CPPUNIT_TEST(runTagIgnoreTest);
  CPPUNIT_TEST(runWordIgnoreTest);
  CPPUNIT_TEST(runBadInputsTest);
  CPPUNIT_TEST(runCustomRuleTest);
  CPPUNIT_TEST(runSchemaValuesOnlyOffTest);
  CPPUNIT_TEST_SUITE_END();

public:


  ImplicitTagRulesDatabaseDeriverTest()
    : HootTestFixture("test-files/schema/ImplicitTagRulesDatabaseDeriverTest/",
                      "test-output/schema/ImplicitTagRulesDatabaseDeriverTest/")
  {
  }

  void runBasicTest()
  {
    const QString input = _inputPath + "ImplicitTagRulesDatabaseDeriverTest-input.implicitTagRules";
    const QString dbOutputFile =
      _outputPath + "ImplicitTagRulesDatabaseDeriverTest-runBasicTest-out.sqlite";

    ImplicitTagRulesDatabaseDeriver rulesDeriver;
    rulesDeriver.setMinTagOccurrencesPerWord(1);
    rulesDeriver.setMinWordLength(1);
    rulesDeriver.setUseSchemaTagValuesForWordsOnly(true);
    rulesDeriver.setCustomRuleFile("");
    rulesDeriver.setTagIgnoreFile("");
    rulesDeriver.setWordIgnoreFile("");
    rulesDeriver.deriveRulesDatabase(input, dbOutputFile);

    ImplicitTagRulesSqliteReader dbReader;
    dbReader.open(dbOutputFile);
    CPPUNIT_ASSERT_EQUAL(86L, dbReader.getRuleCount());
    dbReader.close();
  }

  void runBadInputsTest()
  {
    ImplicitTagRulesDatabaseDeriver rulesDeriver;
    rulesDeriver.setMinTagOccurrencesPerWord(1);
    rulesDeriver.setMinWordLength(1);
    rulesDeriver.setUseSchemaTagValuesForWordsOnly(true);
    rulesDeriver.setCustomRuleFile("");
    rulesDeriver.setTagIgnoreFile("");
    rulesDeriver.setWordIgnoreFile("");

    QString exceptionMsg("");

    try
    {
      rulesDeriver.deriveRulesDatabase(
        _inputPath + "ImplicitTagRulesDatabaseDeriverTest-input.txt",
        _outputPath + "ImplicitTagRulesDatabaseDeriverTest-runBadInputsTest1-out.sqlite");
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(
      exceptionMsg.contains(
        "A *.implicitTagRules file must be the input to implicit tag rules derivation"));

    try
    {
      rulesDeriver.deriveRulesDatabase(
        _inputPath + "ImplicitTagRulesDatabaseDeriverTest-input.implicitTagRules",
        _outputPath + "ImplicitTagRulesDatabaseDeriverTest-runBadInputsTest2-out.txt");
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.contains("Incorrect output specified"));
  }

  void runMinTagOccurrencePerWordTest()
  {
    const QString input = _inputPath + "ImplicitTagRulesDatabaseDeriverTest-input.implicitTagRules";
    const QString dbOutputFile =
      _outputPath + "ImplicitTagRulesDatabaseDeriverTest-runMinTagOccurrencePerWordTest-out.sqlite";

    ImplicitTagRulesDatabaseDeriver rulesDeriver;
    rulesDeriver.setMinTagOccurrencesPerWord(4);
    rulesDeriver.setMinWordLength(1);
    rulesDeriver.setUseSchemaTagValuesForWordsOnly(true);
    rulesDeriver.setCustomRuleFile("");
    rulesDeriver.setTagIgnoreFile("");
    rulesDeriver.setWordIgnoreFile("");
    rulesDeriver.deriveRulesDatabase(input, dbOutputFile);

    ImplicitTagRulesSqliteReader dbReader;
    dbReader.open(dbOutputFile);
    CPPUNIT_ASSERT_EQUAL(1L, dbReader.getRuleCount());
    dbReader.close();
  }

  void runMinWordLengthTest()
  {
    const QString input = _inputPath + "ImplicitTagRulesDatabaseDeriverTest-input.implicitTagRules";
    const QString dbOutputFile =
      _outputPath + "ImplicitTagRulesDatabaseDeriverTest-runMinWordLengthTest-out.sqlite";

    ImplicitTagRulesDatabaseDeriver rulesDeriver;
    rulesDeriver.setMinTagOccurrencesPerWord(1);
    rulesDeriver.setMinWordLength(10);
    rulesDeriver.setUseSchemaTagValuesForWordsOnly(true);
    rulesDeriver.setCustomRuleFile("");
    rulesDeriver.setTagIgnoreFile("");
    rulesDeriver.setWordIgnoreFile("");
    rulesDeriver.deriveRulesDatabase(input, dbOutputFile);

    ImplicitTagRulesSqliteReader dbReader;
    dbReader.open(dbOutputFile);
    CPPUNIT_ASSERT_EQUAL(51L, dbReader.getRuleCount());
    dbReader.close();
  }

  void runTagIgnoreTest()
  {
    const QString input = _inputPath + "ImplicitTagRulesDatabaseDeriverTest-input.implicitTagRules";
    const QString dbOutputFile =
      _outputPath + "ImplicitTagRulesDatabaseDeriverTest-runTagIgnoreTest-out.sqlite";

    ImplicitTagRulesDatabaseDeriver rulesDeriver;
    rulesDeriver.setMinTagOccurrencesPerWord(1);
    rulesDeriver.setMinWordLength(1);
    rulesDeriver.setUseSchemaTagValuesForWordsOnly(true);
    rulesDeriver.setCustomRuleFile("");
    rulesDeriver.setTagIgnoreFile(
      _inputPath + "ImplicitTagRulesDatabaseDeriverTest-tag-ignore-list");
    rulesDeriver.setWordIgnoreFile("");
    rulesDeriver.deriveRulesDatabase(input, dbOutputFile);

    ImplicitTagRulesSqliteReader dbReader;
    dbReader.open(dbOutputFile);
    CPPUNIT_ASSERT_EQUAL(57L, dbReader.getRuleCount());
    dbReader.close();
  }

  void runWordIgnoreTest()
  {
    const QString input = _inputPath + "ImplicitTagRulesDatabaseDeriverTest-input.implicitTagRules";
    const QString dbOutputFile =
      _outputPath + "ImplicitTagRulesDatabaseDeriverTest-runWordIgnoreTest-out.sqlite";

    ImplicitTagRulesDatabaseDeriver rulesDeriver;
    rulesDeriver.setMinTagOccurrencesPerWord(1);
    rulesDeriver.setMinWordLength(1);
    rulesDeriver.setUseSchemaTagValuesForWordsOnly(true);
    rulesDeriver.setCustomRuleFile("");
    rulesDeriver.setTagIgnoreFile("");
    rulesDeriver.setCustomRuleFile("");
    rulesDeriver.setWordIgnoreFile(
      _inputPath + "ImplicitTagRulesDatabaseDeriverTest-word-ignore-list");
    rulesDeriver.deriveRulesDatabase(input, dbOutputFile);

    ImplicitTagRulesSqliteReader dbReader;
    dbReader.open(dbOutputFile);
    CPPUNIT_ASSERT_EQUAL(78L, dbReader.getRuleCount());
    dbReader.close();
  }

  void runCustomRuleTest()
  {
    const QString input = _inputPath + "ImplicitTagRulesDatabaseDeriverTest-input.implicitTagRules";
    const QString dbOutputFile =
      _outputPath + "ImplicitTagRulesDatabaseDeriverTest-runCustomRuleTest-out.sqlite";

    ImplicitTagRulesDatabaseDeriver rulesDeriver;
    rulesDeriver.setMinTagOccurrencesPerWord(1);
    rulesDeriver.setMinWordLength(1);
    rulesDeriver.setUseSchemaTagValuesForWordsOnly(true);
    rulesDeriver.setCustomRuleFile("");
    rulesDeriver.setTagIgnoreFile("");
    rulesDeriver.setCustomRuleFile(
      _inputPath + "ImplicitTagRulesDatabaseDeriverTest-custom-rules-list");
    rulesDeriver.setWordIgnoreFile("");
    rulesDeriver.deriveRulesDatabase(input, dbOutputFile);

    ImplicitTagRulesSqliteReader dbReader;
    dbReader.open(dbOutputFile);
    CPPUNIT_ASSERT_EQUAL(89L, dbReader.getRuleCount());
    dbReader.close();
  }

  void runSchemaValuesOnlyOffTest()
  {
    const QString input = _inputPath + "ImplicitTagRulesDatabaseDeriverTest-input.implicitTagRules";
    const QString dbOutputFile =
      _outputPath + "ImplicitTagRulesDatabaseDeriverTest-runSchemaValuesOnlyOffTest-out.sqlite";

    ImplicitTagRulesDatabaseDeriver rulesDeriver;
    rulesDeriver.setMinTagOccurrencesPerWord(1);
    rulesDeriver.setMinWordLength(1);
    rulesDeriver.setUseSchemaTagValuesForWordsOnly(false);
    rulesDeriver.setCustomRuleFile("");
    rulesDeriver.setTagIgnoreFile("");
    rulesDeriver.setCustomRuleFile("");
    rulesDeriver.setWordIgnoreFile("");
    rulesDeriver.deriveRulesDatabase(input, dbOutputFile);

    ImplicitTagRulesSqliteReader dbReader;
    dbReader.open(dbOutputFile);
    CPPUNIT_ASSERT_EQUAL(404L, dbReader.getRuleCount());
    dbReader.close();
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ImplicitTagRulesDatabaseDeriverTest, "quick");
//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ImplicitTagRulesDatabaseDeriverTest, "serial");

}
