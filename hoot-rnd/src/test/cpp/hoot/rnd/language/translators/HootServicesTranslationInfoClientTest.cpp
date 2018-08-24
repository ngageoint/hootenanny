
// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// hoot
#include <hoot/core/TestUtils.h>
#include <hoot/rnd/language/translators/HootServicesTranslationInfoClient.h>

// Qt
#include <QNetworkRequest>

// Std
#include <sstream>

namespace hoot
{

class HootServicesTranslationInfoClientTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(HootServicesTranslationInfoClientTest);
  CPPUNIT_TEST(runBuildRequestTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runBuildRequestTest()
  {
    HootServicesTranslationInfoClient uut;

    std::stringstream requestStrStrm;
    boost::shared_ptr<QNetworkRequest> request =
      uut._getAvailableLanguagesRequest(
        "http://localhost/test", QStringList("TikaLanguageDetector"), requestStrStrm);

    HOOT_STR_EQUALS("http://localhost/test", request->url().toString());
    HOOT_STR_EQUALS(
      "application/json", request->header(QNetworkRequest::ContentTypeHeader).toString());
    HOOT_STR_EQUALS("", requestStrStrm.str());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(HootServicesTranslationInfoClientTest, "quick");

}


