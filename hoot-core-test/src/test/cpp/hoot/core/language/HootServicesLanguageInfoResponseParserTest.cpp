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
#include <hoot/core/language/HootServicesLanguageInfoResponseParser.h>
#include <hoot/core/language/HootServicesLanguageInfoMockClient.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/StringUtils.h>

namespace hoot
{

class HootServicesLanguageInfoResponseParserTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(HootServicesLanguageInfoResponseParserTest);
  CPPUNIT_TEST(runParseLangsResponseTest);
  CPPUNIT_TEST(runParseAppsResponseTest);
  CPPUNIT_TEST(runLangCodesToLangsTest);
  CPPUNIT_TEST_SUITE_END();

public:

  HootServicesLanguageInfoResponseParserTest()
    : HootTestFixture("test-files/language/HootServicesLanguageInfoResponseParserTest/",
                      UNUSED_PATH)
  {
  }

  void runParseLangsResponseTest()
  {
    std::shared_ptr<boost::property_tree::ptree> response =
      StringUtils::jsonStringToPropTree(HootServicesLanguageInfoMockClient::LANGS_STR);

    QString responseStr =
      HootServicesLanguageInfoResponseParser::parseAvailableLanguagesResponse(
        "detectable", response);
    LOG_VART(responseStr);
    HOOT_STR_EQUALS(
      FileUtils::readFully(_inputPath + "runParseLangsResponseTest-detectable").trimmed(),
      responseStr.trimmed());

    responseStr =
      HootServicesLanguageInfoResponseParser::parseAvailableLanguagesResponse(
        "translatable", response);
    LOG_VART(responseStr);
    HOOT_STR_EQUALS(
      FileUtils::readFully(_inputPath + "runParseLangsResponseTest-translatable").trimmed(),
      responseStr.trimmed());
  }

  void runParseAppsResponseTest()
  {
    std::shared_ptr<boost::property_tree::ptree> response =
      StringUtils::jsonStringToPropTree(HootServicesLanguageInfoMockClient::DETECTORS_STR);
    QString responseStr =
      HootServicesLanguageInfoResponseParser::parseAvailableAppsResponse(
        "detectors", response);
    LOG_VART(responseStr);
    HOOT_STR_EQUALS(
      FileUtils::readFully(_inputPath + "runParseAppsResponseTest-detectors").trimmed(),
      responseStr.trimmed());

    response =
      StringUtils::jsonStringToPropTree(HootServicesLanguageInfoMockClient::TRANSLATORS_STR);
    responseStr =
      HootServicesLanguageInfoResponseParser::parseAvailableAppsResponse(
        "translators", response);
    LOG_VART(responseStr);
    HOOT_STR_EQUALS(
      FileUtils::readFully(_inputPath + "runParseAppsResponseTest-translators").trimmed(),
      responseStr.trimmed());
  }

  void runLangCodesToLangsTest()
  {
    std::shared_ptr<boost::property_tree::ptree> response =
      StringUtils::jsonStringToPropTree(HootServicesLanguageInfoMockClient::LANGS_STR);
    const QMap<QString, QString> langCodesToLangs =
      HootServicesLanguageInfoResponseParser::getLangCodesToLangs(response);

    CPPUNIT_ASSERT_EQUAL(2, langCodesToLangs.size());
    HOOT_STR_EQUALS("German", langCodesToLangs["de"]);
    HOOT_STR_EQUALS("Spanish", langCodesToLangs["es"]);
  }
};

#ifdef HOOT_HAVE_SERVICES
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(HootServicesLanguageInfoResponseParserTest, "quick");
#endif  // HOOT_HAVE_SERVICES

}


