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
#include <hoot/rnd/schema/PoiImplicitTagRawRulesDeriver.h>
#include <hoot/rnd/io/ImplicitTagRulesSqliteReader.h>

// Qt
#include <QDir>
#include <QTemporaryFile>

namespace hoot
{

class PoiImplicitTagRawRulesDeriverTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(PoiImplicitTagRawRulesDeriverTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST(runMultipleInputsTest);
  CPPUNIT_TEST(runDuplicateWordKeyCountTest);
  //TODO: fix; for some strange reason, even though the output is identical to the gold file,
  //HOOT_FILE_EQUALS says it isn't
  //CPPUNIT_TEST(runNameCaseTest);
  //TODO
  //CPPUNIT_TEST(runInputTranslationScriptSizeMismatchTest);
  //CPPUNIT_TEST(runEqualsInNameTest);
  //CPPUNIT_TEST(runNoNameTokenizationTest);
  //CPPUNIT_TEST(runBadInputsTest);
  CPPUNIT_TEST_SUITE_END();

public:

  static QString inDir() { return "test-files/schema/PoiImplicitTagRawRulesDeriverTest"; }
  static QString outDir() { return "test-output/schema/PoiImplicitTagRawRulesDeriverTest"; }

  void runBasicTest()
  {
    QDir().mkpath(outDir());
    QStringList inputs;
    inputs.append(inDir() + "/yemen-crop-2.osm.pbf");
    const QString outputFile =
      outDir() + "/PoiImplicitTagRawRulesDeriverTest-runBasicTest-out.implicitTagRules";

    QStringList translationScripts;
    translationScripts.append("translations/OSM_Ingest.js");

    PoiImplicitTagRawRulesDeriver rulesGenerator;
    rulesGenerator.setConfiguration(conf());
    rulesGenerator.setKeepTempFiles(false); //set true for debugging
    rulesGenerator.deriveRawRules(inputs, translationScripts, outputFile);

    HOOT_FILE_EQUALS(
      inDir() + "/PoiImplicitTagRawRulesGeneratorTest-runBasicTest.implicitTagRules", outputFile);
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

    const QString outputFile =
      outDir() + "/PoiImplicitTagRawRulesDeriverTest-runMultipleInputsTest-out.implicitTagRules";

    PoiImplicitTagRawRulesDeriver rulesGenerator;
    rulesGenerator.setConfiguration(conf());
    rulesGenerator.setKeepTempFiles(false); //set true for debugging
    rulesGenerator.deriveRawRules(inputs, translationScripts, outputFile);

    HOOT_FILE_EQUALS(
      inDir() + "/PoiImplicitTagRawRulesDeriverTest-runMultipleInputsTest.implicitTagRules",
      outputFile);
  }

  void runDuplicateWordKeyCountTest()
  {
    DisableLog dl;
    QDir().mkpath(outDir());
    //TODO: remove output temp file by wildcard here

    boost::shared_ptr<QTemporaryFile> sortedCountFile(
      new QTemporaryFile(
        outDir() +
        "/PoiImplicitTagRawRulesDeriverTest-runDuplicateWordKeyCountTest-sortedCountsInput-XXXXXX"));
    sortedCountFile->setAutoRemove(false);
    if (!sortedCountFile->open())
    {
      throw HootException(
        QObject::tr("Error opening %1 for writing.").arg(sortedCountFile->fileName()));
    }
    QString line = "5\thall\tamenity=hall\n";
    sortedCountFile->write(line.toUtf8());
    line = "5\thall\tamenity=public_hall\n";
    sortedCountFile->write(line.toUtf8());
    line = "4\tschool\tamenity=school\n";
    sortedCountFile->write(line.toUtf8());
    line = "4\tschool\tbuilding=school\n";
    sortedCountFile->write(line.toUtf8());
    sortedCountFile->close();
    sortedCountFile->open();

    PoiImplicitTagRawRulesDeriver rulesGenerator;
    rulesGenerator.setConfiguration(conf());
    rulesGenerator.setTempFileDir(outDir());
    rulesGenerator._sortedCountFile = sortedCountFile;
    rulesGenerator.setKeepTempFiles(true);
    rulesGenerator._removeDuplicatedKeyTypes();
    rulesGenerator._resolveCountTies();

    HOOT_FILE_EQUALS(
      inDir() + "/PoiImplicitTagRawRulesDeriverTest-runDuplicateWordKeyCountTest-deduped-output",
      rulesGenerator._tieResolvedCountFile->fileName());
  }

  void runNameCaseTest()
  {
    //Case is actually already handled correctly in runBasicTest, but this smaller input dataset
    //will make debugging case problems easier, if needed.

    QDir().mkpath(outDir());

    QStringList inputs;
    inputs.append(inDir() + "/PoiImplicitTagRawRulesDeriverTest-runNameCaseTest.osm");

    QStringList translationScripts;
    translationScripts.append("translations/OSM_Ingest.js");

    const QString outputFile =
      outDir() + "/PoiImplicitTagRawRulesDeriverTest-runNameCaseTest-out.implicitTagRules";

    PoiImplicitTagRawRulesDeriver rulesGenerator;
    rulesGenerator.setConfiguration(conf());
    rulesGenerator.setKeepTempFiles(false); //set true for debugging
    rulesGenerator.deriveRawRules(inputs, translationScripts, outputFile);

    HOOT_FILE_EQUALS(
      inDir() + "/PoiImplicitTagRawRulesDeriverTest-runNameCaseTest.implicitTagRules", outputFile);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PoiImplicitTagRawRulesDeriverTest, "quick");

}
