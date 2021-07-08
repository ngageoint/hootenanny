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
 * @copyright Copyright (C) 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/rnd/visitors/NonEnglishLanguageDetectionVisitor.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/util/FileUtils.h>

namespace hoot
{

class NonEnglishLanguageDetectionVisitorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(NonEnglishLanguageDetectionVisitorTest);
  CPPUNIT_TEST(runDetectTest);
  CPPUNIT_TEST(runIgnorePreTranslatedTagsTest);
  CPPUNIT_TEST(runNoTagKeysTest);
  CPPUNIT_TEST(runNamesTest);
  CPPUNIT_TEST(runNamesTestWithAdditionalTagKeys);
  CPPUNIT_TEST_SUITE_END();

public:



  NonEnglishLanguageDetectionVisitorTest()
    : HootTestFixture("test-files/visitors/NonEnglishLanguageDetectionVisitorTest/",
                      "test-output/visitors/NonEnglishLanguageDetectionVisitorTest/")
  {
    setResetType(ResetBasic);
  }

  void runDetectTest()
  {
    const QString testName = "runDetectTest";
    Settings conf = _getDefaultConfig();
    _runDetectTest(
      conf,
      _outputPath + testName + ".osm",
      _inputPath + testName + "-gold.osm");
  }

  void runIgnorePreTranslatedTagsTest()
  {
    const QString testName = "runIgnorePreTranslatedTagsTest";
    Settings conf = _getDefaultConfig();
    conf.set("language.ignore.pre.translated.tags", true);
    std::shared_ptr<NonEnglishLanguageDetectionVisitor> visitor =
      _runDetectTest(
        conf,
        _outputPath + testName + ".osm",
        _inputPath + testName + "-gold.osm");

    const QString detectionSummaryFile =
      _outputPath + "runIgnorePreTranslatedTagsTest-DetectionSummary-out";
    LOG_VART(visitor->_getLangCountsSortedByLangName());
    FileUtils::writeFully(detectionSummaryFile, visitor->_getLangCountsSortedByLangName() + "\n");
    HOOT_FILE_EQUALS(
      _inputPath + "runIgnorePreTranslatedTagsTest-DetectionSummary-gold",
      detectionSummaryFile);
  }

  void runNoTagKeysTest()
  {
    const QString testName = "runNoTagKeysTest";
    Settings conf = _getDefaultConfig();
    conf.set("language.tag.keys", QStringList());
    QString exceptionMsg("");
    try
    {
      _runDetectTest(
        conf,
        _outputPath + testName + ".osm",
        _inputPath + testName + "-gold.osm");
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.startsWith("No tag keys specified"));
  }

  void runNamesTest()
  {
    const QString testName = "runNamesTest";
    Settings conf = _getDefaultConfig();
    conf.set("language.parse.names", true);
    _runDetectTest(
      conf,
      _outputPath + testName + ".osm",
      _inputPath + testName + "-gold.osm");
  }

  void runNamesTestWithAdditionalTagKeys()
  {
    const QString testName = "runNamesTestWithAdditionalTagKeys";
    Settings conf = _getDefaultConfig();
    conf.set("language.parse.names", true);
    conf.set("language.tag.keys", "tag1;tag2");
    _runDetectTest(
      conf,
      _outputPath + testName + ".osm",
      _inputPath + testName + "-gold.osm");
  }

private:

  Settings _getDefaultConfig()
  {
    Settings conf;

    conf.set("language.skip.words.in.english.dictionary", true);
    conf.set("language.ignore.pre.translated.tags", false);
    QStringList tagKeys;
    tagKeys.append("name");
    tagKeys.append("alt_name");
    conf.set("language.tag.keys", tagKeys);
    conf.set("language.detection.detector", "hoot::HootServicesLanguageDetectorMockClient");
    conf.set("language.info.provider", "hoot::HootServicesLanguageInfoMockClient");
    conf.set("language.hoot.services.detection.min.confidence.threshold", "none");
    conf.set("language.detection.write.detected.lang.tags", "true");

    return conf;
  }

  std::shared_ptr<NonEnglishLanguageDetectionVisitor> _runDetectTest(Settings config,
                                                                     const QString& outputFile,
                                                                     const QString& goldFile)
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(
      map, "test-files/visitors/ToEnglishTranslationVisitorTest/ToEnglishTranslationVisitorTest.osm",
      false, Status::Unknown1);

    std::shared_ptr<NonEnglishLanguageDetectionVisitor> visitor(
      new NonEnglishLanguageDetectionVisitor());
    visitor->setConfiguration(config);

    map->visitRw(*visitor);

    OsmMapWriterFactory::write(map, outputFile);

    HOOT_FILE_EQUALS(goldFile, outputFile);

    return visitor;
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(NonEnglishLanguageDetectionVisitorTest, "quick");

}


