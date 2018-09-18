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
#include <hoot/rnd/language/translators/HootServicesTranslatorMockClient.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/StringUtils.h>

namespace hoot
{

static const QString testInputRoot =
  "test-files/language/translators/HootServicesTranslatorClientTest";

class HootServicesTranslatorClientTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(HootServicesTranslatorClientTest);
  CPPUNIT_TEST(runBuildRequestTest);
  CPPUNIT_TEST(runParseResponseTest);
  CPPUNIT_TEST(runInvalidLangDetectConfigTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runBuildRequestTest()
  {
    boost::shared_ptr<HootServicesTranslatorClient> uut = _getClient();

    std::stringstream requestStrStrm;
    boost::shared_ptr<QNetworkRequest> request =
      uut->_getTranslateRequest("text to translate", requestStrStrm);

    HOOT_STR_EQUALS("http://localhost/test", request->url().toString());
    HOOT_STR_EQUALS(
      "application/json", request->header(QNetworkRequest::ContentTypeHeader).toString());
    HOOT_STR_EQUALS(
      FileUtils::readFully(testInputRoot + "/runBuildRequestTest").trimmed(),
      QString::fromStdString(requestStrStrm.str()).trimmed());
  }

  void runParseResponseTest()
  {
    boost::shared_ptr<HootServicesTranslatorClient> uut = _getClient();

    //see comment in StringUtilsTest::jsonParseTest about the formatting of this string
    const QString jsonStr = "{ \"translatedText\": \"How%20old%20are%20you\", \"performExhaustiveTranslationSearchWithNoDetection\": false, \"detectorUsed\": \"TikaLanguageDetector\", \"sourceLangCodes\": [ \"de\",\"es\" ], \"detectedLangCode\": \"de\", \"detectedLangAvailableForTranslation\": true, \"translator\": \"HootLanguageTranslator\", \"sourceText\": \"wie alt bist du\", \"detectedLang\": \"German\", \"detectedLanguageOverridesSpecifiedSourceLanguages\": false }";
    boost::shared_ptr<boost::property_tree::ptree> response =
      StringUtils::jsonStringToPropTree(jsonStr);
    uut->_parseTranslateResponse(response);

    HOOT_STR_EQUALS("How old are you", uut->getTranslatedText());
    CPPUNIT_ASSERT(uut->detectionMade());
    HOOT_STR_EQUALS("German", uut->_detectedLang);
    HOOT_STR_EQUALS("TikaLanguageDetector", uut->_detectorUsed);
    CPPUNIT_ASSERT(uut->_detectedLangAvailableForTranslation);
  }

  void runInvalidLangDetectConfigTest()
  {
    HootServicesTranslatorClient client;
    QStringList sourceLangs;
    sourceLangs.append("detect");
    sourceLangs.append("es");

    QString exceptionMsg("");
    try
    {
      client.setSourceLanguages(sourceLangs);
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT_EQUAL(
      QString("When specifying 'detect' in source languages, no other languages may be specified.")
        .toStdString(),
      exceptionMsg.toStdString());
  }

private:

  boost::shared_ptr<HootServicesTranslatorClient> _getClient()
  {
    boost::shared_ptr<HootServicesTranslatorClient> client(new HootServicesTranslatorMockClient());

    Settings conf;
    conf.set("language.translation.hoot.services.translation.endpoint", "http://localhost/test");
    conf.set("language.translation.hoot.services.translator", "HootLanguageTranslator");
    conf.set("language.translation.hoot.services.detectors", QStringList("TikaLanguageDetector"));
    conf.set("language.translation.detected.language.overrides.specified.source.languages", false);
    conf.set("language.translation.perform.exhaustive.search.with.no.detection", true);
    client->setConfiguration(conf);

    QStringList sourceLangs;
    sourceLangs.append("de");
    sourceLangs.append("es");
    client->setSourceLanguages(sourceLangs);

    return client;
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(HootServicesTranslatorClientTest, "quick");

}


