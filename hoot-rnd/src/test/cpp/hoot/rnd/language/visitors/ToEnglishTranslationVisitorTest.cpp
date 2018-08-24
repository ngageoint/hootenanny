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
 * @copyright Copyright (C) 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/rnd/language/visitors/ToEnglishTranslationVisitor.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>

namespace hoot
{

class ToEnglishTranslationVisitorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ToEnglishTranslationVisitorTest);
  CPPUNIT_TEST(runTranslateTest);
  CPPUNIT_TEST(runNoSourceLangsTest);
  CPPUNIT_TEST(runSkipPreTranslatedTagsTest);
  CPPUNIT_TEST(runSkipWordsInEnglishDictTest);
  CPPUNIT_TEST_SUITE_END();

public:

  static QString testInputRoot;
  static QString testOutputRoot;

  ToEnglishTranslationVisitorTest()
  {
    testInputRoot =
      "test-files/language/visitors/ToEnglishTranslationVisitorTest";
    testOutputRoot =
      "test-files/language/visitors/ToEnglishTranslationVisitorTest";

    setResetType(ResetBasic);
    TestUtils::mkpath(testOutputRoot);
  }

  void runTranslateTest()
  {
    const QString testName = "runTranslateTest";
    _runTranslationTest(
      _getDefaultConfig(),
      testOutputRoot + "/" + testName + ".osm",
      testInputRoot + "/" + testName + "-gold.osm");
  }

  void runNoSourceLangsTest()
  {
    const QString testName = "runNoSourceLangsTest";
    Settings conf = _getDefaultConfig();
    conf.set("language.translation.source.languages", QStringList());
    QString exceptionMsg("");
    try
    {
      _runTranslationTest(
        conf,
        testOutputRoot + "/" + testName + ".osm",
        testInputRoot + "/" + testName + "-gold.osm");
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT_EQUAL(
      QString("Cannot determine source language.").toStdString(), exceptionMsg.toStdString());
  }

  void runSkipPreTranslatedTagsTest()
  {
    const QString testName = "runSkipPreTranslatedTagsTest";
    Settings conf = _getDefaultConfig();
    conf.set("language.translation.skip.pre.translated.tags", true);
    _runTranslationTest(
      conf,
      testOutputRoot + "/" + testName + ".osm",
      testInputRoot + "/" + testName + "-gold.osm");
  }

  void runSkipWordsInEnglishDictTest()
  {
    const QString testName = "runSkipWordsInEnglishDictTest";
    Settings conf = _getDefaultConfig();
    conf.set("language.translation.skip.words.in.english.dictionary", false);
    _runTranslationTest(
      conf,
      testOutputRoot + "/" + testName + ".osm",
      testInputRoot + "/" + testName + "-gold.osm");
  }

private:

  Settings _getDefaultConfig()
  {
    Settings conf;

    conf.set("language.translation.skip.words.in.english.dictionary", true);
    conf.set("language.translation.skip.pre.translated.tags", false);
    QStringList sourceLangs;
    sourceLangs.append("de");
    sourceLangs.append("es");
    conf.set("language.translation.source.languages", sourceLangs);
    QStringList toTranslateTagKeys;
    toTranslateTagKeys.append("name");
    toTranslateTagKeys.append("alt_name");
    conf.set("language.translation.to.translate.tag.keys", toTranslateTagKeys);
    conf.set("language.translation.translator", "hoot::HootServicesTranslatorMockClient");
    conf.set("language.translation.info.provider", "hoot::HootServicesTranslationInfoMockClient");

    return conf;
  }

  void _runTranslationTest(Settings config, const QString outputFile, const QString goldFile)
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, testInputRoot + "/ToEnglishTranslationVisitorTest.osm", false, Status::Unknown1);

    ToEnglishTranslationVisitor visitor;
    visitor.setConfiguration(config);

    map->visitRw(visitor);

    OsmMapWriterFactory::getInstance().write(map, outputFile);

    HOOT_FILE_EQUALS(goldFile, outputFile);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ToEnglishTranslationVisitorTest, "quick");

}


