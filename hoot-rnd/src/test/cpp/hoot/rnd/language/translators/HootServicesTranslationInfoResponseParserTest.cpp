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
#include <hoot/core/TestUtils.h>
#include <hoot/rnd/language/translators/HootServicesTranslationInfoResponseParser.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/StringUtils.h>

namespace hoot
{

static const QString testInputRoot =
  "test-files/language/translators/HootServicesTranslationInfoResponseParserTest";

class HootServicesTranslationInfoResponseParserTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(HootServicesTranslationInfoResponseParserTest);
  CPPUNIT_TEST(runParseLangsResponseTest);
  CPPUNIT_TEST(runParseAppsResponseTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runParseLangsResponseTest()
  {
    const QString jsonStr = "{\"languages\":[{\"name\":\"Arabic\",\"available\":false,\"iso6391code\":\"ar\",\"iso6392code\":\"ara\"},{\"name\":\"Amharic\",\"available\":false,\"iso6391code\":\"am\",\"iso6392code\":\"afr\"}]}";
    boost::shared_ptr<boost::property_tree::ptree> response =
      StringUtils::jsonStringToPropTree(jsonStr);

    QString responseStr =
      HootServicesTranslationInfoResponseParser::parseAvailableLanguagesResponse(
        "detectable", response);
    //LOG_VARD(responseStr);
    HOOT_STR_EQUALS(
      FileUtils::readFully(testInputRoot + "/runParseLangsResponseTest-detectable").trimmed(),
      responseStr.trimmed());

    responseStr =
      HootServicesTranslationInfoResponseParser::parseAvailableLanguagesResponse(
        "translatable", response);
    //LOG_VARD(responseStr);
    HOOT_STR_EQUALS(
      FileUtils::readFully(testInputRoot + "/runParseLangsResponseTest-translatable").trimmed(),
      responseStr.trimmed());
  }

  void runParseAppsResponseTest()
  {
    const QString jsonStr =
      "{\"apps\":[{\"name\":\"TikaLanguageDetector\",\"description\":\"blah\",\"url\":\"https://tika.apache.org\"},{\"name\":\"OpenNlpLanguageDetector\",\"description\":\"more blah\",\"url\":\"https://opennlp.apache.org\"}]}";
    boost::shared_ptr<boost::property_tree::ptree> response =
      StringUtils::jsonStringToPropTree(jsonStr);

    QString responseStr =
      HootServicesTranslationInfoResponseParser::parseAvailableAppsResponse(
        "detectors", response);
    //LOG_VARD(responseStr);
    HOOT_STR_EQUALS(
      FileUtils::readFully(testInputRoot + "/runParseAppsResponseTest-detectors").trimmed(),
      responseStr.trimmed());

    responseStr =
      HootServicesTranslationInfoResponseParser::parseAvailableAppsResponse(
        "translators", response);
    //LOG_VARD(responseStr);
    HOOT_STR_EQUALS(
      FileUtils::readFully(testInputRoot + "/runParseAppsResponseTest-translators").trimmed(),
      responseStr.trimmed());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(HootServicesTranslationInfoResponseParserTest, "quick");

}


