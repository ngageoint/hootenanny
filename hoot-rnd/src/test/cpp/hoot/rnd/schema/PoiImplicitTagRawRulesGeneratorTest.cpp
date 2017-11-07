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
#include <hoot/rnd/schema/PoiImplicitTagRawRulesGenerator.h>
#include <hoot/rnd/io/ImplicitTagRulesSqliteReader.h>

// Qt
#include <QDir>

namespace hoot
{

class PoiImplicitTagRawRulesGeneratorTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(PoiImplicitTagRawRulesGeneratorTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST(runMultipleInputsTest);
  CPPUNIT_TEST(runNameCaseTest);
  //TODO
  //CPPUNIT_TEST(runInputTranslationScriptSizeMismatchTest);
  //CPPUNIT_TEST(runEqualsInNameTest);
  CPPUNIT_TEST_SUITE_END();

public:

  static QString inDir() { return "test-files/io/PoiImplicitTagRawRulesGeneratorTest"; }
  static QString outDir() { return "test-output/io/PoiImplicitTagRawRulesGeneratorTest"; }

  void runBasicTest()
  {
    QDir().mkpath(outDir());

    QStringList inputs;
    inputs.append(inDir() + "/yemen-crop-2.osm.pbf");
    const QString jsonOutputFile =
      outDir() + "/PoiImplicitTagRawRulesGeneratorTest-runBasicTest-out.json";
    const QString dbOutputFile =
      outDir() + "/PoiImplicitTagRawRulesGeneratorTest-runBasicTest-out.sqlite";

    QStringList translationScripts;
    translationScripts.append("translations/OSM_Ingest.js");

    QStringList outputs;
    outputs.append(jsonOutputFile);
    outputs.append(dbOutputFile);

    PoiImplicitTagRawRulesGenerator().generateRules(inputs, translationScripts, outputs);

    HOOT_FILE_EQUALS(inDir() + "/PoiImplicitTagRawRulesGeneratorTest-runBasicTest.json", jsonOutputFile);

    ImplicitTagRulesSqliteReader dbReader;
    dbReader.open(dbOutputFile);
    CPPUNIT_ASSERT_EQUAL(438L, dbReader.getRuleWordPartCount());
    dbReader.close();
  }

  void runMultipleInputsTest()
  {
    QDir().mkpath(outDir());

    QStringList inputs;
    inputs.append(inDir() + "/yemen-crop-2.osm.pbf");
    inputs.append(inDir() + "/philippines-1.osm.pbf");

    QStringList translationScripts;
    translationScripts.append("translations/OSM_Ingest.js");
    translationScripts.append("translations/OSM_Ingest.js");

    const QString jsonOutputFile =
      outDir() + "/PoiImplicitTagRawRulesGeneratorTest-runMultipleInputsTest-out.json";
    const QString dbOutputFile =
      outDir() + "/PoiImplicitTagRawRulesGeneratorTest-runMultipleInputsTest-out.sqlite";
    QStringList outputs;
    outputs.append(jsonOutputFile);
    outputs.append(dbOutputFile);

    PoiImplicitTagRawRulesGenerator().generateRules(inputs, translationScripts, outputs);

    HOOT_FILE_EQUALS(
      inDir() + "/PoiImplicitTagRawRulesGeneratorTest-runMultipleInputsTest.json", jsonOutputFile);

    ImplicitTagRulesSqliteReader dbReader;
    dbReader.open(dbOutputFile);
    CPPUNIT_ASSERT_EQUAL(599L, dbReader.getRuleWordPartCount());
    dbReader.close();
  }

  void runNameCaseTest()
  {
    //Case is actually already handled correctly in runBasicTest, but this smaller input dataset
    //will make debugging case problems easier, if needed.

    QDir().mkpath(outDir());

    QStringList inputs;
    inputs.append(inDir() + "/PoiImplicitTagRawRulesGeneratorTest-runNameCaseTest.osm");

    QStringList translationScripts;
    translationScripts.append("translations/OSM_Ingest.js");

    const QString jsonOutputFile =
      outDir() + "/PoiImplicitTagRawRulesGeneratorTest-runNameCaseTest-out.json";
    const QString dbOutputFile =
      outDir() + "/PoiImplicitTagRawRulesGeneratorTest-runNameCaseTest-out.sqlite";
    QStringList outputs;
    outputs.append(jsonOutputFile);
    outputs.append(dbOutputFile);

    PoiImplicitTagRawRulesGenerator().generateRules(inputs, translationScripts, outputs);

    HOOT_FILE_EQUALS(
      inDir() + "/PoiImplicitTagRulesDeriverTest-runNameCaseTest.json", jsonOutputFile);

    ImplicitTagRulesSqliteReader dbReader;
    dbReader.open(dbOutputFile);
    CPPUNIT_ASSERT_EQUAL(9L, dbReader.getRuleWordPartCount());
    dbReader.close();
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PoiImplicitTagRawRulesGeneratorTest, "quick");

}
