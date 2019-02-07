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
 * @copyright Copyright (C) 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/rnd/visitors/ToEnglishTranslationComparisonVisitor.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>

namespace hoot
{

static const QString testInputRoot =
  "test-files/visitors/ToEnglishTranslationVisitorTest";
static const QString goldInputRoot =
  "test-files/visitors/ToEnglishTranslationComparisonVisitorTest";
static const QString testOutputRoot =
  "test-output/visitors/ToEnglishTranslationComparisonVisitorTest";

class ToEnglishTranslationComparisonVisitorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ToEnglishTranslationComparisonVisitorTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST(runInvalidSourceLangsTest);
  CPPUNIT_TEST(runInvalidSourceLangsTest2);
  CPPUNIT_TEST_SUITE_END();

public:

  ToEnglishTranslationComparisonVisitorTest()
  {
    setResetType(ResetBasic);
    TestUtils::mkpath(testOutputRoot);
  }

  void runTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, testInputRoot + "/ToEnglishTranslationVisitorTest.osm", false, Status::Unknown1);

    ToEnglishTranslationComparisonVisitor visitor;
    visitor.setConfiguration(_getDefaultConfig());

    map->visitRw(visitor);

    const QString outputFile = testOutputRoot + "/runTest.osm";
    OsmMapWriterFactory::write(map, outputFile);

    HOOT_FILE_EQUALS(goldInputRoot + "/runTest-gold.osm", outputFile);
  }

  void runInvalidSourceLangsTest()
  {
    ToEnglishTranslationComparisonVisitor visitor;
    Settings conf;
    QStringList sourceLangs;
    sourceLangs.append("de");
    sourceLangs.append("es");
    conf.set("language.translation.source.languages", sourceLangs);

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
    conf.set("language.translation.source.languages", sourceLangs);

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

    conf.set("language.skip.words.in.english.dictionary", true);
    conf.set("language.ignore.pre.translated.tags", false);
    QStringList sourceLangs;
    sourceLangs.append("de");
    conf.set("language.translation.source.languages", sourceLangs);
    QStringList preTranslateTagKeys;
    preTranslateTagKeys.append("name:en");
    preTranslateTagKeys.append("alt_name:en");
    conf.set("language.translation.comparison.pretranslated.tag.keys", preTranslateTagKeys);
    QStringList toTranslateTagKeys;
    toTranslateTagKeys.append("name");
    toTranslateTagKeys.append("alt_name");
    conf.set("language.tag.keys", toTranslateTagKeys);
    conf.set("language.translation.translator", "hoot::HootServicesTranslatorMockClient");
    conf.set("language.info.provider", "hoot::HootServicesLanguageInfoMockClient");
    conf.set("hoot.services.auth.user.name", "ToEnglishTranslationComparisonVisitorTest");
    conf.set("hoot.services.auth.access.token", "testAccessToken");
    conf.set("hoot.services.auth.access.token.secret", "testAccessTokenSecret");

    return conf;
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ToEnglishTranslationComparisonVisitorTest, "quick");

}


