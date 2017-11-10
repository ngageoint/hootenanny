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
  //CPPUNIT_TEST(runBasicTest);
  //CPPUNIT_TEST(runExplicitTagsTest);
  CPPUNIT_TEST(runMinTagOccurrencePerWordTest);
  //TODO
  //CPPUNIT_TEST(runBadInputsTest);
  //CPPUNIT_TEST(runCustomRuleFileTest);
  //CPPUNIT_TEST(runMinWordLengthTest);
  //CPPUNIT_TEST(runRuleIgnoreTest);
  //CPPUNIT_TEST(runTagIgnoreTest);
  //CPPUNIT_TEST(runWordIgnoreTest);
  CPPUNIT_TEST_SUITE_END();

public:

  static QString inDir() { return "test-files/io/PoiImplicitTagRulesDeriverTest"; }
  static QString outDir() { return "test-output/io/PoiImplicitTagRulesDeriverTest"; }

  void runBasicTest()
  {
    QDir().mkpath(outDir());

    const QString input = inDir() + "/PoiImplicitTagRulesDeriverTest-input.implicitTagRules";
    const QString jsonOutputFile =
      outDir() + "/PoiImplicitTagRulesDeriverTest-runBasicTest-out.json";
    const QString dbOutputFile =
      outDir() + "/PoiImplicitTagRulesDeriverTest-runBasicTest-out.sqlite";

    QStringList outputs;
    outputs.append(jsonOutputFile);
    outputs.append(dbOutputFile);

    PoiImplicitTagRulesDeriver rulesDeriver;
    rulesDeriver.setConfiguration(conf());
    rulesDeriver.deriveRules(input, outputs);

    HOOT_FILE_EQUALS(inDir() + "/PoiImplicitTagRulesDeriverTest-runBasicTest.json", jsonOutputFile);

    ImplicitTagRulesSqliteReader dbReader;
    dbReader.open(dbOutputFile);
    CPPUNIT_ASSERT_EQUAL(438L, dbReader.getRuleWordPartCount());
    dbReader.close();
  }

  //TODO: add key=value test
  void runExplicitTagsTest()
  {
    QDir().mkpath(outDir());

    const QString input = inDir() + "/PoiImplicitTagRulesDeriverTest-input.implicitTagRules";

    const QString jsonOutputFile =
      outDir() + "/PoiImplicitTagRulesDeriverTest-runExplicitTagsTest-out.json";
    const QString dbOutputFile =
      outDir() + "/PoiImplicitTagRulesDeriverTest-runExplicitTagsTest-out.sqlite";
    QStringList outputs;
    outputs.append(jsonOutputFile);
    outputs.append(dbOutputFile);

    PoiImplicitTagRulesDeriver rulesDeriver;
    rulesDeriver.setConfiguration(conf());
    rulesDeriver.setTagFile(inDir() + "/PoiImplicitTagRulesDeriverTest-tag-list");
    rulesDeriver.deriveRules(input, outputs);

    HOOT_FILE_EQUALS(
      inDir() + "/PoiImplicitTagRulesDeriverTest-runExplicitTagsTest.json", jsonOutputFile);

    ImplicitTagRulesSqliteReader dbReader;
    dbReader.open(dbOutputFile);
    CPPUNIT_ASSERT_EQUAL(247L, dbReader.getRuleWordPartCount());
    dbReader.close();
  }

  void runMinTagOccurrencePerWordTest()
  {
    QDir().mkpath(outDir());

    const QString input = inDir() + "/PoiImplicitTagRulesDeriverTest-input.implicitTagRules";

    const QString jsonOutputFile =
      outDir() + "/PoiImplicitTagRulesDeriverTest-runMinTagOccurrencePerWordTest-out.json";
    const QString dbOutputFile =
      outDir() + "/PoiImplicitTagRulesDeriverTest-runMinTagOccurrencePerWordTest-out.sqlite";
    QStringList outputs;
    outputs.append(jsonOutputFile);
    outputs.append(dbOutputFile);

    PoiImplicitTagRulesDeriver rulesDeriver;
    rulesDeriver.setConfiguration(conf());
    rulesDeriver.setMinTagOccurrencesPerWord(4);
    rulesDeriver.deriveRules(input, outputs);

    HOOT_FILE_EQUALS(
      inDir() + "/PoiImplicitTagRulesDeriverTest-runMinTagOccurrencePerWordTest.json",
      jsonOutputFile);

    ImplicitTagRulesSqliteReader dbReader;
    dbReader.open(dbOutputFile);
    CPPUNIT_ASSERT_EQUAL(109L, dbReader.getRuleWordPartCount());
    dbReader.close();
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PoiImplicitTagRulesDeriverTest, "quick");

}
