
// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// hoot
#include <hoot/core/TestUtils.h>
#include <hoot/rnd/language/translators/HootServicesTranslatorClient.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/StringUtils.h>

namespace hoot
{

class HootServicesTranslatorClientTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(HootServicesTranslatorClientTest);
  CPPUNIT_TEST(runBuildRequestTest);
  CPPUNIT_TEST(runParseResponseTest);
  CPPUNIT_TEST_SUITE_END();

public:

  static QString testInputRoot;

  HootServicesTranslatorClientTest()
  {
    testInputRoot = "test-files/language/translators/HootServicesTranslatorClientTest";
  }

  void runBuildRequestTest()
  {
    HootServicesTranslatorClient uut;
    uut.setConfiguration(_getSettings());

    std::stringstream requestStrStrm;
    boost::shared_ptr<QNetworkRequest> request =
      uut._getTranslateRequest("text to translate", requestStrStrm);

    HOOT_STR_EQUALS("http://localhost/test", request->url().toString());
    HOOT_STR_EQUALS(
      "application/json", request->header(QNetworkRequest::ContentTypeHeader).toString());
    HOOT_STR_EQUALS("", requestStrStrm.str());
  }

  void runParseResponseTest()
  {
    HootServicesTranslatorClient uut;
    uut.setConfiguration(_getSettings());

    const QString jsonStr = "{\"translatedText\":\"How%20old%20are%20you\",\"performExhaustiveTranslationSearchWithNoDetection\":false,\"detectorUsed\":\"TikaLanguageDetector\",\"sourceLangCodes\":[\"de\",\"es\"],\"detectedLangCode\":\"de\",\"detectedLangAvailableForTranslation\":true,\"translator\":\"HootLanguageTranslator\",\"sourceText\":\"wie alt bist du\",\"detectedLang\":\"German\",\"detectedLanguageOverridesSpecifiedSourceLanguages\":false}";
    boost::shared_ptr<boost::property_tree::ptree> response =
      StringUtils::jsonStringToPropTree(jsonStr);
    uut._parseTranslateResponse(response);

    HOOT_STR_EQUALS("How old are you", uut.getTranslatedText());
    CPPUNIT_ASSERT(uut.detectionMade());
    HOOT_STR_EQUALS("German", uut._detectedLang);
    HOOT_STR_EQUALS("TikaLanguageDetector", uut._detectorUsed);
    CPPUNIT_ASSERT(uut._detectedLangAvailableForTranslation);
  }

private:

  Settings _getSettings()
  {
    Settings conf;
    conf.set("language.translation.hoot.services.translation.endpoint", "http://localhost/test");
    conf.set("language.translation.hoot.services.translator", "HootLanguageTranslator");
    conf.set("language.translation.hoot.services.detectors", QStringList("TikaLanguageDetector"));
    conf.set("language.translation.detected.language.overrides.specified.source.languages", false);
    conf.set("language.translation.perform.exhaustive.search.with.no.detection", true);
    return conf;
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(HootServicesTranslatorClientTest, "quick");

}


