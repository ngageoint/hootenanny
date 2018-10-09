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
 * @copyright Copyright (C) 2013, 2014, 2015, 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/auth/HootServicesLoginManager.h>
#include <hoot/core/io/ServicesDbTestUtils.h>
#include <hoot/core/io/HootApiDb.h>
#include <hoot/core/util/UuidHelper.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

namespace hoot
{

class ServicesHootServicesLoginManagerTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ServicesHootServicesLoginManagerTest);
  CPPUNIT_TEST(getRequestTokenTest);
  CPPUNIT_TEST(loginRequestTest);
  CPPUNIT_TEST(loginResponseTest);
  CPPUNIT_TEST(getAccessTokensTest);
  CPPUNIT_TEST(getAccessTokensInvalidUserIdTest);
  CPPUNIT_TEST(logoutTest);
  CPPUNIT_TEST(logoutInvalidTokensTest);
  CPPUNIT_TEST(logoutInvalidUserTest);
  CPPUNIT_TEST_SUITE_END();

public:

  virtual void setUp()
  {
    _sessionId = UuidHelper::createUuid().toString().replace("{", "").replace("}", "");

    _userId =
      ServicesDbTestUtils::insertTestUser(
        "ServicesHootServicesLoginManagerTest",
        "ServicesHootServicesLoginManagerTest@hoot.com",
        _sessionId, "testAccessToken", "testAccessTokenSecret");

    _cookies =
      ServicesDbTestUtils::getTestSessionCookie(
        "testSessionId", ConfigOptions().getHootServicesAuthRequestTokenEndpoint());
  }

  virtual void tearDown()
  {
    ServicesDbTestUtils::deleteUserByUserName("ServicesHootServicesLoginManagerTest");
  }

  ServicesHootServicesLoginManagerTest()
  {
  }

  void getRequestTokenTest()
  {
    QString authUrl;
    const QString responseToken = HootServicesLoginManager().getRequestToken(authUrl);
    CPPUNIT_ASSERT(!responseToken.isEmpty());
    CPPUNIT_ASSERT(authUrl.startsWith("https://www.openstreetmap.org/oauth/authorize?oauth_token="));
    CPPUNIT_ASSERT(authUrl.endsWith(responseToken));
  }

  void loginRequestTest()
  {
    HootServicesLoginManager uut;
    uut._cookies = _cookies;

    QUrl loginUrl;
    HootNetworkRequest request = uut._getLoginRequest("testRequestToken", "testVerifier", loginUrl);

    //check that the request has the single session cookie with the correct session id
    CPPUNIT_ASSERT_EQUAL(1, request.getCookies()->size());
    HOOT_STR_EQUALS(_cookies->toString(), request.getCookies()->toString());

    HOOT_STR_EQUALS(
      "http://localhost:8080/hoot-services/auth/oauth1/verify?oauth_token=testRequestToken&oauth_verifier=testVerifier",
      loginUrl.toString());
  }

  void loginResponseTest()
  {
    //id is the only thing we parse from the user json
    CPPUNIT_ASSERT_EQUAL(
      1L,
      HootServicesLoginManager()._parseLoginResponse(
        "{\"email\":\"1@hootenanny\",\"id\":1,\"display_name\":\"icenine18\",\"provider_created_at\":1268772678000,\"last_authorized\":1539029718227,\"created_at\":1539029718190}"));
  }

  void getAccessTokensTest()
  {
    QString accessToken;
    QString accessTokenSecret;
    HootServicesLoginManager().getAccessTokens(_userId, accessToken, accessTokenSecret);

    HOOT_STR_EQUALS("testAccessToken", accessToken);
    HOOT_STR_EQUALS("testAccessTokenSecret", accessTokenSecret);
  }

  void getAccessTokensInvalidUserIdTest()
  {
    QString exceptionMsg("");
    QString accessToken;
    QString accessTokenSecret;
    try
    {
      HootServicesLoginManager().getAccessTokens(-1, accessToken, accessTokenSecret);
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
      LOG_VART(exceptionMsg);
    }
    CPPUNIT_ASSERT(exceptionMsg.contains("User does not exist."));
  }

  void logoutTest()
  {
    //Since we're deleting this user as part of this test, don't use the same test user as the
    //other tests.
    const QString sessionId = UuidHelper::createUuid().toString().replace("{", "").replace("}", "");
    const long userId =
      ServicesDbTestUtils::insertTestUser(
        "ServicesHootServicesLoginManagerTest2", "ServicesHootServicesLoginManagerTest2@hoot.com",
        sessionId, "testAccessToken2", "testAccessTokenSecret2");
    LOG_VART(userId);

    CPPUNIT_ASSERT(
      HootServicesLoginManager().logout(
        "ServicesHootServicesLoginManagerTest2", "testAccessToken2", "testAccessTokenSecret2"));

    HootApiDb db;
    QUrl url(HootApiDb::getBaseUrl().toString() + "/blah");
    db.open(url);
    CPPUNIT_ASSERT(!db.userExists(userId));
  }

  void logoutInvalidTokensTest()
  {
    QString exceptionMsg("");
    try
    {
      HootServicesLoginManager().logout(
        "ServicesHootServicesLoginManagerTest", "testAccessTokenBLAH", "testAccessTokenSecret");
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
      LOG_VART(exceptionMsg);
    }
    CPPUNIT_ASSERT(exceptionMsg.contains("Invalid access tokens."));
    exceptionMsg = "";

    try
    {
      HootServicesLoginManager().logout(
        "ServicesHootServicesLoginManagerTest", "testAccessToken", "testAccessTokenSecretBLAH");
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
      LOG_VART(exceptionMsg);
    }
    CPPUNIT_ASSERT(exceptionMsg.contains("Invalid access tokens."));
  }

  void logoutInvalidUserTest()
  {
    QString exceptionMsg("");
    try
    {
      HootServicesLoginManager().logout(
        "ServicesHootServicesLoginManagerTestBLAH", "testAccessToken", "testAccessTokenSecret");
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
      LOG_VART(exceptionMsg);
    }
    CPPUNIT_ASSERT(exceptionMsg.contains("User does not exist."));
  }

private:

  QString _sessionId;
  long _userId;
  boost::shared_ptr<HootNetworkCookieJar> _cookies;
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ServicesHootServicesLoginManagerTest, "slow");

}
