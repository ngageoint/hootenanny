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

// hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/language/HootServicesLanguageInfoMockClient.h>
#include <hoot/core/language/HootServicesTranslatorMockClient.h>
#include <hoot/core/visitors/ToEnglishTranslationVisitor.h>

namespace hoot
{

class ToEnglishTranslationVisitorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ToEnglishTranslationVisitorTest);
  CPPUNIT_TEST(runTranslateTest);
  CPPUNIT_TEST(runNoSourceLangsTest);
  CPPUNIT_TEST(runIgnorePreTranslatedTagsTest);
  CPPUNIT_TEST(runNoTagKeysTest);
  CPPUNIT_TEST(runNamesTest);
  CPPUNIT_TEST(runNamesTestWithAdditionalTagKeys);
  CPPUNIT_TEST_SUITE_END();

public:

  ToEnglishTranslationVisitorTest()
    : HootTestFixture("test-files/visitors/ToEnglishTranslationVisitorTest/",
                      "test-output/visitors/ToEnglishTranslationVisitorTest/")
  {
  }

  void runTranslateTest()
  {
    const QString testName = "runTranslateTest";
    _runTranslationTest(
      _getDefaultConfig(),
      _outputPath + testName + ".osm",
      _inputPath + testName + "-gold.osm");
  }

  void runNoSourceLangsTest()
  {
    const QString testName = "runNoSourceLangsTest";
    Settings conf = _getDefaultConfig();
    conf.set(ConfigOptions::getLanguageTranslationSourceLanguagesKey(), QStringList());
    QString exceptionMsg("");
    try
    {
      _runTranslationTest(
        conf,
        _outputPath + testName + ".osm",
        _inputPath + testName + "-gold.osm");
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    HOOT_STR_EQUALS(QString("No source languages populated."), exceptionMsg);
  }

  void runIgnorePreTranslatedTagsTest()
  {
    const QString testName = "runIgnorePreTranslatedTagsTest";
    Settings conf = _getDefaultConfig();
    conf.set(ConfigOptions::getLanguageIgnorePreTranslatedTagsKey(), true);
    _runTranslationTest(
      conf,
      _outputPath + testName + ".osm",
      _inputPath + testName + "-gold.osm");
  }

  void runNoTagKeysTest()
  {
    const QString testName = "runNoTagKeysTest";
    Settings conf = _getDefaultConfig();
    conf.set(ConfigOptions::getLanguageTagKeysKey(), QStringList());
    QString exceptionMsg("");
    try
    {
      _runTranslationTest(
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
    conf.set(ConfigOptions::getLanguageParseNamesKey(), true);
    _runTranslationTest(
      conf,
      _outputPath + testName + ".osm",
      _inputPath + testName + "-gold.osm");
  }

  void runNamesTestWithAdditionalTagKeys()
  {
    const QString testName = "runNamesTestWithAdditionalTagKeys";
    Settings conf = _getDefaultConfig();
    conf.set(ConfigOptions::getLanguageParseNamesKey(), true);
    conf.set(ConfigOptions::getLanguageTagKeysKey(), "tag1;tag2");
    _runTranslationTest(
      conf,
      _outputPath + testName + ".osm",
      _inputPath + testName + "-gold.osm");
  }

private:

  Settings _getDefaultConfig()
  {
    Settings conf;

    conf.set(ConfigOptions::getLanguageSkipWordsInEnglishDictionaryKey(), true);
    conf.set(ConfigOptions::getLanguageIgnorePreTranslatedTagsKey(), false);
    QStringList sourceLangs;
    sourceLangs.append("de");
    sourceLangs.append("es");
    conf.set(ConfigOptions::getLanguageTranslationSourceLanguagesKey(), sourceLangs);
    QStringList toTranslateTagKeys;
    toTranslateTagKeys.append("name");
    toTranslateTagKeys.append("alt_name");
    conf.set(ConfigOptions::getLanguageTagKeysKey(), toTranslateTagKeys);
    conf.set(
      ConfigOptions::getLanguageTranslationTranslatorKey(),
      HootServicesTranslatorMockClient::className());
    conf.set(
      ConfigOptions::getLanguageInfoProviderKey(), HootServicesLanguageInfoMockClient::className());

    return conf;
  }

  void _runTranslationTest(const Settings& config, const QString& outputFile, const QString& goldFile)
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(
      map, _inputPath + "ToEnglishTranslationVisitorTest.osm", false, Status::Unknown1);

    ToEnglishTranslationVisitor visitor;
    visitor.setConfiguration(config);

    map->visitRw(visitor);

    OsmMapWriterFactory::write(map, outputFile);

    HOOT_FILE_EQUALS(goldFile, outputFile);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ToEnglishTranslationVisitorTest, "quick");

}


