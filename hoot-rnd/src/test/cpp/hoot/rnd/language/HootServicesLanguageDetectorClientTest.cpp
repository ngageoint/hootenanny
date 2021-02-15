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
#include <hoot/core/TestUtils.h>
#include <hoot/rnd/language/HootServicesLanguageDetectorMockClient.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/rnd/language/LanguageDetectionConfidenceLevel.h>

namespace hoot
{

class HootServicesLanguageDetectorClientTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(HootServicesLanguageDetectorClientTest);
  CPPUNIT_TEST(runRequestDataTest);
  CPPUNIT_TEST(runParseResponseTest);
  CPPUNIT_TEST(runConfidenceTest);
  CPPUNIT_TEST_SUITE_END();

public:

  HootServicesLanguageDetectorClientTest()
    : HootTestFixture("test-files/language/HootServicesLanguageDetectorClientTest/",
                      UNUSED_PATH)
  {
  }

  void runRequestDataTest()
  {
    std::shared_ptr<HootServicesLanguageDetectorClient> uut = _getClient();

    HOOT_STR_EQUALS(
      FileUtils::readFully(_inputPath + "runRequestDataTest").trimmed(),
      uut->_getRequestData("text to detect").trimmed());
  }

  void runParseResponseTest()
  {
    std::shared_ptr<HootServicesLanguageDetectorClient> uut = _getClient();
    std::shared_ptr<boost::property_tree::ptree> response =
      StringUtils::jsonStringToPropTree(_responseStr);

    QString detectorUsed;
    HOOT_STR_EQUALS("de", uut->_parseResponse(response, detectorUsed));
    HOOT_STR_EQUALS("TikaLanguageDetector", detectorUsed);
  }

  void runConfidenceTest()
  {
    std::shared_ptr<HootServicesLanguageDetectorClient> uut = _getClient();
    uut->_minConfidence = LanguageDetectionConfidenceLevel::High;

    std::shared_ptr<boost::property_tree::ptree> response =
      StringUtils::jsonStringToPropTree(_responseStr);

    QString detectorUsed;
    HOOT_STR_EQUALS("", uut->_parseResponse(response, detectorUsed));
    HOOT_STR_EQUALS("TikaLanguageDetector", detectorUsed);
  }

private:

  // see comment in StringUtilsTest::jsonParseTest about the formatting of this string
  const QString _responseStr =
    "{ \"detectorUsed\": \"TikaLanguageDetector\", "
      "\"detectedLangCode\": \"de\", "
      "\"sourceText\": \"wie%20alt%20bist%20du\", "
      "\"detectedLang\": \"German\", "
      "\"detectionConfidence\": \"medium\" }";

  std::shared_ptr<HootServicesLanguageDetectorClient> _getClient()
  {
    std::shared_ptr<HootServicesLanguageDetectorClient> client(
      new HootServicesLanguageDetectorMockClient());

    Settings conf;
    conf.set("hoot.services.auth.host", "localhost");
    conf.set("hoot.services.auth.port", "8080");
    conf.set("language.hoot.services.detectors", QStringList("TikaLanguageDetector"));
    conf.set("language.hoot.services.detection.min.confidence.threshold", "low");
    client->setConfiguration(conf);

    return client;
  }
};

#ifdef HOOT_HAVE_SERVICES
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(HootServicesLanguageDetectorClientTest, "quick");
#endif  // HOOT_HAVE_SERVICES

}


