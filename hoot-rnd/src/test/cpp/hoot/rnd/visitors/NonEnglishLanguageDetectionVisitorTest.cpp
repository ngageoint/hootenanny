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
#include <hoot/rnd/visitors/NonEnglishLanguageDetectionVisitor.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>

namespace hoot
{

static const QString testInputRoot =
  "test-files/visitors/ToEnglishTranslationVisitorTest";
static const QString testInputRoot2 =
  "test-files/visitors/NonEnglishLanguageDetectionVisitorTest";
static const QString testOutputRoot =
  "test-output/visitors/NonEnglishLanguageDetectionVisitor";

class NonEnglishLanguageDetectionVisitorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(NonEnglishLanguageDetectionVisitorTest);
  CPPUNIT_TEST(runDetectTest);
  CPPUNIT_TEST(runIgnorePreTranslatedTagsTest);
  CPPUNIT_TEST(runDetectWithTagWriteTest);
  CPPUNIT_TEST_SUITE_END();

public:

  NonEnglishLanguageDetectionVisitorTest()
  {
    setResetType(ResetBasic);
    TestUtils::mkpath(testOutputRoot);
  }

  void runDetectTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, testInputRoot + "/ToEnglishTranslationVisitorTest.osm", false, Status::Unknown1);

    NonEnglishLanguageDetectionVisitor visitor;
    visitor.setConfiguration(_getDefaultConfig());

    map->visitRw(visitor);

    CPPUNIT_ASSERT_EQUAL(5L, visitor._numTagDetectionsMade); //4
    CPPUNIT_ASSERT_EQUAL(3L, visitor._numElementsWithSuccessfulTagDetection); //3
    CPPUNIT_ASSERT_EQUAL(4L, visitor._numTotalElements); //4
    CPPUNIT_ASSERT_EQUAL(5L, visitor._numProcessedTags); //7
    CPPUNIT_ASSERT_EQUAL(4L, visitor._numProcessedElements); //3

    CPPUNIT_ASSERT_EQUAL(2, visitor._langCounts.size());
    CPPUNIT_ASSERT_EQUAL(1, visitor._langCounts["de"]);
    CPPUNIT_ASSERT_EQUAL(1, visitor._langCounts["es"]);
    CPPUNIT_ASSERT(!visitor._langCounts.contains("en"));
  }

  void runDetectWithTagWriteTest()
  {
    const QString testName = "runDetectWithTagWriteTest";
    Settings conf = _getDefaultConfig();
    conf.set("language.detection.write.detected.lang.tags", true);
    _runDetectTest(
      conf,
      testOutputRoot + "/" + testName + ".osm",
      testInputRoot2 + "/" + testName + "-gold.osm");
  }

  void runIgnorePreTranslatedTagsTest()
  {
    const QString testName = "runIgnorePreTranslatedTagsTest";
    Settings conf = _getDefaultConfig();
    conf.set("language.ignore.pre.translated.tags", true);
    conf.set("language.detection.write.detected.lang.tags", true);
    _runDetectTest(
      conf,
      testOutputRoot + "/" + testName + ".osm",
      testInputRoot2 + "/" + testName + "-gold.osm");
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
    conf.set("language.info.provider", "hoot::HootServicesTranslationInfoMockClient");

    return conf;
  }

  void _runDetectTest(Settings config, const QString outputFile, const QString goldFile)
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, testInputRoot + "/ToEnglishTranslationVisitorTest.osm", false, Status::Unknown1);

    NonEnglishLanguageDetectionVisitor visitor;
    visitor.setConfiguration(config);

    map->visitRw(visitor);

    OsmMapWriterFactory::getInstance().write(map, outputFile);

    HOOT_FILE_EQUALS(goldFile, outputFile);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(NonEnglishLanguageDetectionVisitorTest, "quick");

}


