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
 * @copyright Copyright (C) 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/rnd/schema/ImplicitTagRawRulesDeriver.h>
#include <hoot/core/io/ImplicitTagRulesSqliteReader.h>

// Qt
#include <QDir>
#include <QTemporaryFile>

namespace hoot
{

class ImplicitTagRawRulesDeriverTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(ImplicitTagRawRulesDeriverTest);

  //TODO: fix these - keep showing up as diff output even though its identical
  //CPPUNIT_TEST(runBasicPoiTest);
  //CPPUNIT_TEST(runTranslateNamesFalsePoiTest);

  CPPUNIT_TEST(runMultipleInputsPoiTest);
  CPPUNIT_TEST(runDuplicateWordKeyCountPoiTest);
  CPPUNIT_TEST(runNameCasePoiTest); 
  CPPUNIT_TEST_SUITE_END();

public:

  static QString inDir() { return "test-files/schema/ImplicitTagRawRulesDeriverTest"; }
  static QString outDir() { return "test-output/schema/ImplicitTagRawRulesDeriverTest"; }

  void runBasicPoiTest()
  {
    TestUtils::mkpath(outDir());

    QStringList inputs;
    inputs.append(inDir() + "/yemen-crop-2.osm.pbf");
    const QString outputFile =
      outDir() + "/ImplicitTagRawRulesDeriverTest-runBasicTest-out.implicitTagRules";

    QStringList translationScripts;
    translationScripts.append("translations/OSM_Ingest.js");
    ImplicitTagRawRulesDeriver rawRulesDeriver;
    rawRulesDeriver.setKeepTempFiles(false); //set true for debugging
    rawRulesDeriver.setSkipFiltering(false);
    rawRulesDeriver.setSortParallelCount(1);
    rawRulesDeriver.setTranslateAllNamesToEnglish(true);
    rawRulesDeriver.setElementFilter("hoot::ImplicitTagEligiblePoiPolyCriterion");
    rawRulesDeriver.deriveRawRules(inputs, translationScripts, outputFile);

    HOOT_FILE_EQUALS(
      inDir() + "/ImplicitTagRawRulesGeneratorTest-runBasicTest.implicitTagRules", outputFile);
  }

  void runMultipleInputsPoiTest()
  {
    TestUtils::mkpath(outDir());

    QStringList inputs;
    inputs.append(inDir() + "/yemen-crop-2.osm.pbf");
    inputs.append(inDir() + "/philippines-1.osm.pbf");

    QStringList translationScripts;
    translationScripts.append("translations/OSM_Ingest.js");
    translationScripts.append("translations/OSM_Ingest.js");

    const QString outputFile =
      outDir() + "/ImplicitTagRawRulesDeriverTest-runMultipleInputsTest-out.implicitTagRules";

    ImplicitTagRawRulesDeriver rawRulesDeriver;
    rawRulesDeriver.setKeepTempFiles(false); //set true for debugging
    rawRulesDeriver.setSkipFiltering(false);
    rawRulesDeriver.setSortParallelCount(1);
    rawRulesDeriver.setTranslateAllNamesToEnglish(true);
    rawRulesDeriver.setElementFilter("hoot::ImplicitTagEligiblePoiPolyCriterion");
    rawRulesDeriver.deriveRawRules(inputs, translationScripts, outputFile);

    HOOT_FILE_EQUALS(
      inDir() + "/ImplicitTagRawRulesDeriverTest-runMultipleInputsTest.implicitTagRules",
      outputFile);
  }

  void runDuplicateWordKeyCountPoiTest()
  {
    DisableLog dl;
    TestUtils::mkpath(outDir());

    boost::shared_ptr<QTemporaryFile> sortedCountFile(
      new QTemporaryFile(
        outDir() +
        "/ImplicitTagRawRulesDeriverTest-runDuplicateWordKeyCountTest-sortedCountsInput-XXXXXX"));
    sortedCountFile->setAutoRemove(false);
    if (!sortedCountFile->open())
    {
      // Note: QTemporaryFile returns empty string for ->fileName() if it can't be opened
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

    ImplicitTagRawRulesDeriver rawRulesDeriver;
    rawRulesDeriver.setTempFileDir(outDir());
    rawRulesDeriver.setKeepTempFiles(true);
    rawRulesDeriver.setSkipFiltering(false);
    rawRulesDeriver.setSortParallelCount(1);
    rawRulesDeriver.setTranslateAllNamesToEnglish(true);
    rawRulesDeriver.setElementFilter("hoot::ImplicitTagEligiblePoiPolyCriterion");
    rawRulesDeriver._sortedCountFile = sortedCountFile;
    rawRulesDeriver._removeDuplicatedKeyTypes();
    rawRulesDeriver._resolveCountTies();

    LOG_VARD(rawRulesDeriver._dedupedCountFile->fileName());
    LOG_VARD(rawRulesDeriver._tieResolvedCountFile->fileName());
    HOOT_FILE_EQUALS(
      inDir() + "/ImplicitTagRawRulesDeriverTest-runDuplicateWordKeyCountTest-deduped-output",
      rawRulesDeriver._tieResolvedCountFile->fileName());
  }

  void runNameCasePoiTest()
  {
    //Case is actually already handled correctly in runBasicTest, but this smaller input dataset
    //will make debugging case problems easier, if needed.

    TestUtils::mkpath(outDir());

    QStringList inputs;
    inputs.append(inDir() + "/ImplicitTagRawRulesDeriverTest-runNameCaseTest.osm");

    QStringList translationScripts;
    translationScripts.append("translations/OSM_Ingest.js");

    const QString outputFile =
      outDir() + "/ImplicitTagRawRulesDeriverTest-runNameCaseTest-out.implicitTagRules";

    ImplicitTagRawRulesDeriver rawRulesDeriver;
    rawRulesDeriver.setKeepTempFiles(false); //set true for debugging
    rawRulesDeriver.setSkipFiltering(false);
    rawRulesDeriver.setSortParallelCount(1);
    rawRulesDeriver.setTranslateAllNamesToEnglish(true);
    rawRulesDeriver.setElementFilter("hoot::ImplicitTagEligiblePoiPolyCriterion");
    rawRulesDeriver.deriveRawRules(inputs, translationScripts, outputFile);

    HOOT_FILE_EQUALS(
      inDir() + "/ImplicitTagRawRulesDeriverTest-runNameCaseTest.implicitTagRules", outputFile);
  }

  void runTranslateNamesFalsePoiTest()
  {
    TestUtils::mkpath(outDir());

    QStringList inputs;
    inputs.append(inDir() + "/yemen-crop-2.osm.pbf");
    const QString outputFile =
      outDir() + "/ImplicitTagRawRulesDeriverTest-runTranslateNamesFalsePoiTest-out.implicitTagRules";

    QStringList translationScripts;
    translationScripts.append("translations/OSM_Ingest.js");
    ImplicitTagRawRulesDeriver rawRulesDeriver;
    rawRulesDeriver.setKeepTempFiles(false); //set true for debugging
    rawRulesDeriver.setSkipFiltering(false);
    rawRulesDeriver.setSortParallelCount(1);
    rawRulesDeriver.setTranslateAllNamesToEnglish(false);
    rawRulesDeriver.setElementFilter("hoot::ImplicitTagEligiblePoiPolyCriterion");
    rawRulesDeriver.deriveRawRules(inputs, translationScripts, outputFile);

    HOOT_FILE_EQUALS(
      inDir() + "/ImplicitTagRawRulesGeneratorTest-runTranslateNamesFalsePoiTest.implicitTagRules",
      outputFile);
  }

  void runBadInputsTest()
  {
    TestUtils::mkpath(outDir());

    QString exceptionMsg("");
    QStringList inputs;
    QStringList translationScripts;
    ImplicitTagRawRulesDeriver rawRulesDeriver;

    rawRulesDeriver.setKeepTempFiles(false); //set true for debugging
    rawRulesDeriver.setSkipFiltering(false);
    rawRulesDeriver.setSortParallelCount(1);
    rawRulesDeriver.setTranslateAllNamesToEnglish(true);

    try
    {
      rawRulesDeriver.setElementFilter("hoot::AreaCriterion");
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.contains("Invalid filter type"));

    inputs.clear();
    inputs.append(inDir() + "/yemen-crop-2.osm.pbf");
    translationScripts.clear();
    translationScripts.append("translations/OSM_Ingest.js");
    translationScripts.append("translations/OSM_Ingest.js");
    try
    {
      rawRulesDeriver.deriveRawRules(
        inputs, translationScripts,
        outDir() + "/ImplicitTagRawRulesDeriverTest-runBadInputsTest-out.implicitTagRules");
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.contains("No element type was specified"));

    rawRulesDeriver.setElementFilter("hoot::ImplicitTagEligiblePoiPolyCriterion");
    inputs.clear();
    inputs.append(inDir() + "/yemen-crop-2.osm.pbf");
    translationScripts.clear();
    translationScripts.append("translations/OSM_Ingest.js");
    translationScripts.append("translations/OSM_Ingest.js");
    try
    {
      rawRulesDeriver.deriveRawRules(
        inputs, translationScripts,
        outDir() + "/ImplicitTagRawRulesDeriverTest-runBadInputsTest-out.implicitTagRules");
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(
      exceptionMsg.contains(
        "The size of the input datasets list must equal the size of the list of translation scripts"));

    inputs.clear();
    translationScripts.clear();
    translationScripts.append("translations/OSM_Ingest.js");
    translationScripts.append("translations/OSM_Ingest.js");
    try
    {
      rawRulesDeriver.deriveRawRules(
        inputs, translationScripts,
        outDir() + "/ImplicitTagRawRulesDeriverTest-runBadInputsTest-out.implicitTagRules");
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.contains("No inputs were specified"));

    inputs.clear();
    inputs.append(inDir() + "/yemen-crop-2.osm.pbf");
    inputs.append(inDir() + "/philippines-1.osm.pbf");
    translationScripts.clear();
    translationScripts.append("translations/OSM_Ingest.js");
    translationScripts.append("translations/OSM_Ingest.js");
    try
    {
      rawRulesDeriver.deriveRawRules(inputs, translationScripts, "");
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.contains("No output was specified"));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ImplicitTagRawRulesDeriverTest, "quick");

}
