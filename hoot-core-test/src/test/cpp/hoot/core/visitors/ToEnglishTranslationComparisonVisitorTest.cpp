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
#include <hoot/core/visitors/ToEnglishTranslationComparisonVisitor.h>

namespace hoot
{


class ToEnglishTranslationComparisonVisitorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ToEnglishTranslationComparisonVisitorTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST(runInvalidSourceLangsTest);
  CPPUNIT_TEST(runInvalidSourceLangsTest2);
  CPPUNIT_TEST_SUITE_END();

public:

  ToEnglishTranslationComparisonVisitorTest()
    : HootTestFixture("test-files/visitors/ToEnglishTranslationComparisonVisitorTest/",
                      "test-output/visitors/ToEnglishTranslationComparisonVisitorTest/")
  {
  }

  void runTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(
      map,
      "test-files/visitors/ToEnglishTranslationVisitorTest/ToEnglishTranslationVisitorTest.osm",
      false,
      Status::Unknown1);

    ToEnglishTranslationComparisonVisitor visitor;
    visitor.setConfiguration(_getDefaultConfig());

    map->visitRw(visitor);

    const QString outputFile = _outputPath + "runTest.osm";
    OsmMapWriterFactory::write(map, outputFile);

    HOOT_FILE_EQUALS(_inputPath + "runTest-gold.osm", outputFile);
  }

  void runInvalidSourceLangsTest()
  {
    ToEnglishTranslationComparisonVisitor visitor;
    Settings conf;
    QStringList sourceLangs;
    sourceLangs.append("de");
    sourceLangs.append("es");
    conf.set(ConfigOptions::getLanguageTranslationSourceLanguagesKey(), sourceLangs);

    QString exceptionMsg("");
    try
    {
      visitor.setConfiguration(conf);
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT_EQUAL(
      QString("ToEnglishTranslationComparisonVisitor supports only one source language and does not support detect mode.")
        .toStdString(),
      exceptionMsg.toStdString());
  }

  void runInvalidSourceLangsTest2()
  {
    ToEnglishTranslationComparisonVisitor visitor;
    Settings conf;
    QStringList sourceLangs;
    sourceLangs.append("detect");
    conf.set(ConfigOptions::getLanguageTranslationSourceLanguagesKey(), sourceLangs);

    QString exceptionMsg("");
    try
    {
      visitor.setConfiguration(conf);
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT_EQUAL(
      QString("ToEnglishTranslationComparisonVisitor supports only one source language and does not support detect mode.")
        .toStdString(),
      exceptionMsg.toStdString());
  }

private:

  Settings _getDefaultConfig()
  {
    Settings conf;

    conf.set(ConfigOptions::getLanguageSkipWordsInEnglishDictionaryKey(), true);
    conf.set(ConfigOptions::getLanguageIgnorePreTranslatedTagsKey(), false);
    QStringList sourceLangs;
    sourceLangs.append("de");
    conf.set(ConfigOptions::getLanguageTranslationSourceLanguagesKey(), sourceLangs);
    QStringList preTranslateTagKeys;
    preTranslateTagKeys.append("name:en");
    preTranslateTagKeys.append("alt_name:en");
    conf.set(ConfigOptions::getLanguageTranslationComparisonPretranslatedTagKeysKey(), preTranslateTagKeys);
    QStringList toTranslateTagKeys;
    toTranslateTagKeys.append("name");
    toTranslateTagKeys.append("alt_name");
    conf.set(ConfigOptions::getLanguageTagKeysKey(), toTranslateTagKeys);
    conf.set(
      ConfigOptions::getLanguageTranslationTranslatorKey(),
      HootServicesTranslatorMockClient::className());
    conf.set(
      ConfigOptions::getLanguageInfoProviderKey(), HootServicesLanguageInfoMockClient::className());
    conf.set(
      ConfigOptions::getHootServicesAuthUserNameKey(), "ToEnglishTranslationComparisonVisitorTest");
    conf.set(ConfigOptions::getHootServicesAuthAccessTokenKey(), "testAccessToken");
    conf.set(ConfigOptions::getHootServicesAuthAccessTokenSecretKey(), "testAccessTokenSecret");

    return conf;
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ToEnglishTranslationComparisonVisitorTest, "quick");

}


