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
    //see comment in StringUtilsTest::jsonParseTest about the formatting of the expected string
    HOOT_STR_EQUALS(
      "{ \"apps\": [ \"TikaLanguageDetector\" ] }",
      QString::fromStdString(requestStrStrm.str()).simplified());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(HootServicesTranslationInfoClientTest, "quick");

}


