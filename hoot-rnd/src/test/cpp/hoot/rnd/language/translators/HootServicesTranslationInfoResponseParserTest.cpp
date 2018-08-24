
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

class HootServicesTranslationInfoResponseParserTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(HootServicesTranslationInfoResponseParserTest);
  CPPUNIT_TEST(runParseLangsResponseTest);
  CPPUNIT_TEST(runParseAppsResponseTest);
  CPPUNIT_TEST_SUITE_END();

public:

  static QString testInputRoot;

  HootServicesTranslationInfoResponseParserTest()
  {
    testInputRoot = "test-files/language/translators/HootServicesTranslatorResponseParserTest";
  }

  void runParseLangsResponseTest()
  {
    const QString jsonStr = "{\"languages\":[{\"name\":\"Arabic\",\"available\":false,\"iso6391code\":\"ar\",\"iso6392code\":\"ara\"},{\"name\":\"Amharic\",\"available\":false,\"iso6391code\":\"am\",\"iso6392code\":\"afr\"}]}";
    boost::shared_ptr<boost::property_tree::ptree> response =
      StringUtils::jsonStringToPropTree(jsonStr);

    HOOT_STR_EQUALS(
      FileUtils::readFully(testInputRoot + "/runPrintLangsTest-detectable"),
      HootServicesTranslationInfoResponseParser::parseAvailableLanguagesResponse(
        "detectable", response));
    HOOT_STR_EQUALS(
      FileUtils::readFully(testInputRoot + "/runPrintLangsTest-translatable"),
      HootServicesTranslationInfoResponseParser::parseAvailableLanguagesResponse(
        "translatable", response));
  }

  void runParseAppsResponseTest()
  {
    const QString jsonStr =
      "{\"apps\":[{\"name\":\"TikaLanguageDetector\",\"description\":\"The language detection portion of a library which detects and extracts metadata and text from many different file types\",\"url\":\"https://tika.apache.org\"},{\"name\":\"OpenNlpLanguageDetector\",\"description\":\"The language detector portion of a machine learning based toolkit for the processing of natural language text\",\"url\":\"https://opennlp.apache.org\"}]}";
    boost::shared_ptr<boost::property_tree::ptree> response =
      StringUtils::jsonStringToPropTree(jsonStr);

    HOOT_STR_EQUALS(
      FileUtils::readFully(testInputRoot + "/runPrintAppsTest-detectors"),
      HootServicesTranslationInfoResponseParser::parseAvailableLanguagesResponse(
        "detectors", response));
    HOOT_STR_EQUALS(
      FileUtils::readFully(testInputRoot + "/runPrintAppsTest-translators"),
      HootServicesTranslationInfoResponseParser::parseAvailableLanguagesResponse(
        "translators", response));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(HootServicesTranslationInfoResponseParserTest, "quick");

}


