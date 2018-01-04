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
 * @copyright Copyright (C) 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/rnd/schema/PoiImplicitTagRulesDeriver.h>
#include <hoot/rnd/io/ImplicitTagRulesSqliteReader.h>

// Qt
#include <QDir>

namespace hoot
{

class PoiImplicitTagRulesDeriverTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(PoiImplicitTagRulesDeriverTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST(runMinTagOccurrencePerWordTest);
  CPPUNIT_TEST(runMinWordLengthTest);
  CPPUNIT_TEST(runTagIgnoreTest);
  CPPUNIT_TEST(runWordIgnoreTest);
  //TODO
  //CPPUNIT_TEST(runBadInputsTest);
  //CPPUNIT_TEST(runCustomRuleFileTest);
  CPPUNIT_TEST_SUITE_END();

public:

  static QString inDir() { return "test-files/schema/PoiImplicitTagRulesDeriverTest"; }
  static QString outDir() { return "test-output/schema/PoiImplicitTagRulesDeriverTest"; }

  void runBasicTest()
  {
    QDir().mkpath(outDir());

    const QString input = inDir() + "/PoiImplicitTagRulesDeriverTest-input.implicitTagRules";
    const QString dbOutputFile =
      outDir() + "/PoiImplicitTagRulesDeriverTest-runBasicTest-out.sqlite";

    PoiImplicitTagRulesDeriver rulesDeriver;
    rulesDeriver.setConfiguration(conf());
    rulesDeriver.setCustomRuleFile("");
    rulesDeriver.setTagIgnoreFile("");
    rulesDeriver.setWordIgnoreFile("");
    rulesDeriver.deriveRules(input, dbOutputFile);

    ImplicitTagRulesSqliteReader dbReader;
    dbReader.open(dbOutputFile);
    CPPUNIT_ASSERT_EQUAL(438L, dbReader.getRuleWordPartCount());
    dbReader.close();
  }

  void runMinTagOccurrencePerWordTest()
  {
    QDir().mkpath(outDir());

    const QString input = inDir() + "/PoiImplicitTagRulesDeriverTest-input.implicitTagRules";
    const QString dbOutputFile =
      outDir() + "/PoiImplicitTagRulesDeriverTest-runMinTagOccurrencePerWordTest-out.sqlite";;

    PoiImplicitTagRulesDeriver rulesDeriver;
    rulesDeriver.setConfiguration(conf());
    rulesDeriver.setMinTagOccurrencesPerWord(4);
    rulesDeriver.setCustomRuleFile("");
    rulesDeriver.setTagIgnoreFile("");
    rulesDeriver.setWordIgnoreFile("");
    rulesDeriver.deriveRules(input, dbOutputFile);

    ImplicitTagRulesSqliteReader dbReader;
    dbReader.open(dbOutputFile);
    CPPUNIT_ASSERT_EQUAL(109L, dbReader.getRuleWordPartCount());
    dbReader.close();
  }

  void runMinWordLengthTest()
  {
    QDir().mkpath(outDir());

    const QString input = inDir() + "/PoiImplicitTagRulesDeriverTest-input.implicitTagRules";
    const QString dbOutputFile =
      outDir() + "/PoiImplicitTagRulesDeriverTest-runMinWordLengthTest-out.sqlite";

    PoiImplicitTagRulesDeriver rulesDeriver;
    rulesDeriver.setConfiguration(conf());
    rulesDeriver.setMinWordLength(10);
    rulesDeriver.setCustomRuleFile("");
    rulesDeriver.setTagIgnoreFile("");
    rulesDeriver.setWordIgnoreFile("");
    rulesDeriver.deriveRules(input, dbOutputFile);

    ImplicitTagRulesSqliteReader dbReader;
    dbReader.open(dbOutputFile);
    CPPUNIT_ASSERT_EQUAL(127L, dbReader.getRuleWordPartCount());
    dbReader.close();
  }

  //TODO: test that ignore overrides explicit list
  void runTagIgnoreTest()
  {
    QDir().mkpath(outDir());

    const QString input = inDir() + "/PoiImplicitTagRulesDeriverTest-input.implicitTagRules";
    const QString dbOutputFile =
      outDir() + "/PoiImplicitTagRulesDeriverTest-runTagIgnoreTest-out.sqlite";

    PoiImplicitTagRulesDeriver rulesDeriver;
    rulesDeriver.setConfiguration(conf());
    rulesDeriver.setTagIgnoreFile(inDir() + "/PoiImplicitTagRulesDeriverTest-tag-ignore-list");
    rulesDeriver.setCustomRuleFile("");
    rulesDeriver.setWordIgnoreFile("");
    rulesDeriver.deriveRules(input, dbOutputFile);

    ImplicitTagRulesSqliteReader dbReader;
    dbReader.open(dbOutputFile);
    CPPUNIT_ASSERT_EQUAL(314L, dbReader.getRuleWordPartCount()); //TODO: is this right?
    dbReader.close();
  }

  //TODO: test that ignore overrides explicit list
  void runWordIgnoreTest()
  {
    QDir().mkpath(outDir());

    const QString input = inDir() + "/PoiImplicitTagRulesDeriverTest-input.implicitTagRules";
    const QString dbOutputFile =
      outDir() + "/PoiImplicitTagRulesDeriverTest-runWordIgnoreTest-out.sqlite";

    PoiImplicitTagRulesDeriver rulesDeriver;
    rulesDeriver.setConfiguration(conf());
    rulesDeriver.setWordIgnoreFile(inDir() + "/PoiImplicitTagRulesDeriverTest-word-ignore-list");
    rulesDeriver.setCustomRuleFile("");
    rulesDeriver.setTagIgnoreFile("");
    rulesDeriver.deriveRules(input, dbOutputFile);

    ImplicitTagRulesSqliteReader dbReader;
    dbReader.open(dbOutputFile);
    CPPUNIT_ASSERT_EQUAL(433L, dbReader.getRuleWordPartCount()); //TODO: is this right?
    dbReader.close();
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PoiImplicitTagRulesDeriverTest, "quick");

}
