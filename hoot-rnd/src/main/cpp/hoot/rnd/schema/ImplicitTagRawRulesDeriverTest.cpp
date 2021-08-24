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
 * @copyright Copyright (C) 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/rnd/io/ImplicitTagRulesSqliteReader.h>
#include <hoot/core/language/ToEnglishDictionaryTranslator.h>
#include <hoot/rnd/schema/ImplicitTagRawRulesDeriver.h>

// Qt
#include <QTemporaryFile>

namespace hoot
{

/*
 * The tests in this class will fail with any translator other than the current default,
 * ToEnglishDictionaryTranslator.
 */
class ImplicitTagRawRulesDeriverTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ImplicitTagRawRulesDeriverTest);
  CPPUNIT_TEST(runBasicPoiTest);
  CPPUNIT_TEST(runTranslateNamesFalsePoiTest);
  CPPUNIT_TEST(runMultipleInputsPoiTest);
  CPPUNIT_TEST(runDuplicateWordKeyCountPoiTest);
  CPPUNIT_TEST(runNameCasePoiTest);
  CPPUNIT_TEST_SUITE_END();

public:

  ImplicitTagRawRulesDeriverTest()
    : HootTestFixture("test-files/schema/ImplicitTagRawRulesDeriverTest/",
                      "test-output/schema/ImplicitTagRawRulesDeriverTest/")
  {
  }

  void runBasicPoiTest()
  {
    QStringList inputs;
    inputs.append(_inputPath + "yemen-crop-2.osm.pbf");
    const QString outputFile =
      _outputPath + "ImplicitTagRawRulesDeriverTest-runBasicTest-out.implicitTagRules";

    QStringList translationScripts;
    translationScripts.append("translations/OSM_Ingest.js");
    ImplicitTagRawRulesDeriver rawRulesDeriver;
    rawRulesDeriver.setKeepTempFiles(false); //set true for debugging
    rawRulesDeriver.setSkipFiltering(false);
    rawRulesDeriver.setSortParallelCount(1);
    rawRulesDeriver.setTranslateNamesToEnglish(true);
    rawRulesDeriver._translator = std::make_shared<ToEnglishDictionaryTranslator>();
    rawRulesDeriver.setElementCriterion("hoot::ImplicitTagEligiblePoiPolyCriterion");
    rawRulesDeriver.deriveRawRules(inputs, translationScripts, outputFile);

    HOOT_FILE_EQUALS(
      _inputPath + "ImplicitTagRawRulesDeriverTest-runBasicTest.implicitTagRules", outputFile);
  }

  void runMultipleInputsPoiTest()
  {
    QStringList inputs;
    inputs.append(_inputPath + "yemen-crop-2.osm.pbf");
    inputs.append(_inputPath + "philippines-1.osm.pbf");

    QStringList translationScripts;
    translationScripts.append("translations/OSM_Ingest.js");
    translationScripts.append("translations/OSM_Ingest.js");

    const QString outputFile =
      _outputPath + "ImplicitTagRawRulesDeriverTest-runMultipleInputsTest-out.implicitTagRules";

    ImplicitTagRawRulesDeriver rawRulesDeriver;
    rawRulesDeriver.setKeepTempFiles(false); //set true for debugging
    rawRulesDeriver.setSkipFiltering(false);
    rawRulesDeriver.setSortParallelCount(1);
    rawRulesDeriver.setTranslateNamesToEnglish(true);
    rawRulesDeriver._translator = std::make_shared<ToEnglishDictionaryTranslator>();
    rawRulesDeriver.setElementCriterion("hoot::ImplicitTagEligiblePoiPolyCriterion");
    rawRulesDeriver.deriveRawRules(inputs, translationScripts, outputFile);

    HOOT_FILE_EQUALS(
      _inputPath + "ImplicitTagRawRulesDeriverTest-runMultipleInputsTest.implicitTagRules",
      outputFile);
  }

  void runDuplicateWordKeyCountPoiTest()
  {
    DisableLog dl;

    std::shared_ptr<QTemporaryFile> sortedCountFile =
      std::make_shared<QTemporaryFile>(
        _outputPath +
        "ImplicitTagRawRulesDeriverTest-runDuplicateWordKeyCountTest-sortedCountsInput-XXXXXX");
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
    rawRulesDeriver.setTempFileDir(_outputPath);
    rawRulesDeriver.setKeepTempFiles(true);
    rawRulesDeriver.setSkipFiltering(false);
    rawRulesDeriver.setSortParallelCount(1);
    rawRulesDeriver.setTranslateNamesToEnglish(true);
    rawRulesDeriver._translator = std::make_shared<ToEnglishDictionaryTranslator>();
    rawRulesDeriver.setElementCriterion("hoot::ImplicitTagEligiblePoiPolyCriterion");
    rawRulesDeriver._sortedCountFile = sortedCountFile;
    rawRulesDeriver._removeDuplicatedKeyTypes();
    rawRulesDeriver._resolveCountTies();

    LOG_VARD(rawRulesDeriver._dedupedCountFile->fileName());
    LOG_VARD(rawRulesDeriver._tieResolvedCountFile->fileName());
    HOOT_FILE_EQUALS(
      _inputPath + "ImplicitTagRawRulesDeriverTest-runDuplicateWordKeyCountTest-deduped-output",
      rawRulesDeriver._tieResolvedCountFile->fileName());
  }

  void runNameCasePoiTest()
  {
    // Case is actually already handled correctly in runBasicTest, but this smaller input dataset
    // will make debugging case problems easier, if needed.

    QStringList inputs;
    inputs.append(_inputPath + "ImplicitTagRawRulesDeriverTest-runNameCaseTest.osm");

    QStringList translationScripts;
    translationScripts.append("translations/OSM_Ingest.js");

    const QString outputFile =
      _outputPath + "ImplicitTagRawRulesDeriverTest-runNameCaseTest-out.implicitTagRules";

    ImplicitTagRawRulesDeriver rawRulesDeriver;
    rawRulesDeriver.setKeepTempFiles(false); //set true for debugging
    rawRulesDeriver.setSkipFiltering(false);
    rawRulesDeriver.setSortParallelCount(1);
    rawRulesDeriver.setTranslateNamesToEnglish(true);
    rawRulesDeriver._translator = std::make_shared<ToEnglishDictionaryTranslator>();
    rawRulesDeriver.setElementCriterion("hoot::ImplicitTagEligiblePoiPolyCriterion");
    rawRulesDeriver.deriveRawRules(inputs, translationScripts, outputFile);

    HOOT_FILE_EQUALS(
      _inputPath + "ImplicitTagRawRulesDeriverTest-runNameCaseTest.implicitTagRules", outputFile);
  }

  void runTranslateNamesFalsePoiTest()
  {
    QStringList inputs;
    inputs.append(_inputPath + "yemen-crop-2.osm.pbf");
    const QString outputFile =
      _outputPath +
      "ImplicitTagRawRulesDeriverTest-runTranslateNamesFalsePoiTest-out.implicitTagRules";

    QStringList translationScripts;
    translationScripts.append("translations/OSM_Ingest.js");
    ImplicitTagRawRulesDeriver rawRulesDeriver;
    rawRulesDeriver.setKeepTempFiles(false); //set true for debugging
    rawRulesDeriver.setSkipFiltering(false);
    rawRulesDeriver.setSortParallelCount(1);
    rawRulesDeriver.setTranslateNamesToEnglish(false);
    rawRulesDeriver._translator = std::make_shared<ToEnglishDictionaryTranslator>();
    rawRulesDeriver.setElementCriterion("hoot::ImplicitTagEligiblePoiPolyCriterion");
    rawRulesDeriver.deriveRawRules(inputs, translationScripts, outputFile);

    HOOT_FILE_EQUALS(
      _inputPath + "ImplicitTagRawRulesDeriverTest-runTranslateNamesFalsePoiTest.implicitTagRules",
      outputFile);
  }

  void runBadInputsTest()
  {
    QString exceptionMsg("");
    QStringList inputs;
    QStringList translationScripts;
    ImplicitTagRawRulesDeriver rawRulesDeriver;

    rawRulesDeriver.setKeepTempFiles(false); //set true for debugging
    rawRulesDeriver.setSkipFiltering(false);
    rawRulesDeriver.setSortParallelCount(1);
    rawRulesDeriver.setTranslateNamesToEnglish(true);
    rawRulesDeriver._translator = std::make_shared<ToEnglishDictionaryTranslator>();

    try
    {
      rawRulesDeriver.setElementCriterion("hoot::AreaCriterion");
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.contains("Invalid criterion type"));

    inputs.clear();
    inputs.append(_inputPath + "yemen-crop-2.osm.pbf");
    translationScripts.clear();
    translationScripts.append("translations/OSM_Ingest.js");
    translationScripts.append("translations/OSM_Ingest.js");
    try
    {
      rawRulesDeriver.deriveRawRules(
        inputs, translationScripts,
        _outputPath + "ImplicitTagRawRulesDeriverTest-runBadInputsTest-out.implicitTagRules");
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.contains("No element type was specified"));

    rawRulesDeriver.setElementCriterion("hoot::ImplicitTagEligiblePoiPolyCriterion");
    inputs.clear();
    inputs.append(_inputPath + "yemen-crop-2.osm.pbf");
    translationScripts.clear();
    translationScripts.append("translations/OSM_Ingest.js");
    translationScripts.append("translations/OSM_Ingest.js");
    try
    {
      rawRulesDeriver.deriveRawRules(
        inputs, translationScripts,
        _outputPath + "ImplicitTagRawRulesDeriverTest-runBadInputsTest-out.implicitTagRules");
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
        _outputPath + "ImplicitTagRawRulesDeriverTest-runBadInputsTest-out.implicitTagRules");
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.contains("No inputs were specified"));

    inputs.clear();
    inputs.append(_inputPath + "yemen-crop-2.osm.pbf");
    inputs.append(_inputPath + "philippines-1.osm.pbf");
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

};
