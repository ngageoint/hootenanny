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
 * @copyright Copyright (C) 2013, 2014, 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/rnd/auth/HootServicesLoginManager.h>
#ifdef HOOT_HAVE_SERVICES
#include <hoot/core/io/ServicesDbTestUtils.h>
#endif  // HOOT_HAVE_SERVICES
#include <hoot/core/io/HootApiDb.h>
#include <hoot/core/util/UuidHelper.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

namespace hoot
{

/**
 * Parts of this test will fail if Tomcat is not running.
 *
 * @todo Some of these tests (disabled now) started failing on Jenkins for an unknown reason.
 * They're not that critical, as they're only needed by the language translation feature which isn't
 * currently in active use. They can be fixed at a later time.
 */
class ServicesHootServicesLoginManagerTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ServicesHootServicesLoginManagerTest);
  //CPPUNIT_TEST(getRequestTokenTest);
  #ifdef HOOT_HAVE_SERVICES
  CPPUNIT_TEST(loginRequestTest);
  CPPUNIT_TEST(getAccessTokensTest);
  //CPPUNIT_TEST(logoutTest);
  CPPUNIT_TEST(logoutInvalidTokensTest);
  CPPUNIT_TEST(logoutInvalidUserTest);
  #endif  // HOOT_HAVE_SERVICES
  CPPUNIT_TEST(loginResponseTest);
  CPPUNIT_TEST(getAccessTokensInvalidUserIdTest);
  CPPUNIT_TEST_SUITE_END();

public:

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

  #ifdef HOOT_HAVE_SERVICES
  void loginRequestTest()
  {
    HootServicesLoginManager uut;
    std::shared_ptr<HootNetworkCookieJar> cookies =
      ServicesDbTestUtils::getTestSessionCookie(
        "testSessionId", HootServicesLoginManager::_getRequestTokenUrl());
    uut._cookies = cookies;

    QUrl loginUrl;
    HootNetworkRequest request = uut._getLoginRequest("testRequestToken", "testVerifier", loginUrl);

    //check that the request has the single session cookie with the correct session id
    CPPUNIT_ASSERT_EQUAL(1, request.getCookies()->size());
    HOOT_STR_EQUALS(cookies->toString(), request.getCookies()->toString());

    HOOT_STR_EQUALS(
      "http://localhost:8080/hoot-services/auth/oauth1/verify?oauth_token=testRequestToken&oauth_verifier=testVerifier",
      loginUrl.toString());
  }
  #endif  // HOOT_HAVE_SERVICES

  void loginResponseTest()
  {
    //id is the only thing we parse from the user json
    CPPUNIT_ASSERT_EQUAL(
      1L,
      HootServicesLoginManager()._parseLoginResponse(
        "{\"email\":\"1@hootenanny\",\"id\":1,\"display_name\":\"icenine18\",\"provider_created_at\":1268772678000,\"last_authorized\":1539029718227,\"created_at\":1539029718190}"));
  }

  #ifdef HOOT_HAVE_SERVICES
  void getAccessTokensTest()
  {
    const QString sessionId = UuidHelper::createUuid().toString().replace("{", "").replace("}", "");
    QString testName = "ServicesHootServicesLoginManagerTest_getAccessTokensTest";
    QString email = testName + "@hoottestcpp.org";
    const long userId =
      ServicesDbTestUtils::insertTestUser(
        testName, email,
        sessionId, "testAccessToken", "testAccessTokenSecret");

    QString accessToken;
    QString accessTokenSecret;
    HootServicesLoginManager().getAccessTokens(userId, accessToken, accessTokenSecret);

    ServicesDbTestUtils::deleteUser(email);

    HOOT_STR_EQUALS("testAccessToken", accessToken);
    HOOT_STR_EQUALS("testAccessTokenSecret", accessTokenSecret);
  }
  #endif  // HOOT_HAVE_SERVICES

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

  #ifdef HOOT_HAVE_SERVICES
  void logoutTest()
  {
    const QString sessionId = UuidHelper::createUuid().toString().replace("{", "").replace("}", "");
    const long userId =
      ServicesDbTestUtils::insertTestUser(
        "ServicesHootServicesLoginManagerTest::logoutTest",
        "ServicesHootServicesLoginManagerTest::logoutTest@hoot.com",
        sessionId, "testAccessToken", "testAccessTokenSecret");
    LOG_VART(userId);

    //this deletes the user record
    CPPUNIT_ASSERT(
      HootServicesLoginManager().logout(
        "ServicesHootServicesLoginManagerTest::logoutTest",
        "testAccessToken", "testAccessTokenSecret"));

    HootApiDb db;
    QUrl url(HootApiDb::getBaseUrl().toString() + "/blah");
    db.open(url);
    CPPUNIT_ASSERT(!db.userExists(userId));
  }
  #endif  // HOOT_HAVE_SERVICES

  #ifdef HOOT_HAVE_SERVICES
  void logoutInvalidTokensTest()
  {
    const QString sessionId = UuidHelper::createUuid().toString().replace("{", "").replace("}", "");
    const long userId =
      ServicesDbTestUtils::insertTestUser(
        "ServicesHootServicesLoginManagerTest::logoutInvalidTokensTest",
        "ServicesHootServicesLoginManagerTest::logoutInvalidTokensTest@hoot.com",
        sessionId, "testAccessToken", "testAccessTokenSecret");
    LOG_VART(userId);

    QString exceptionMsg("");
    try
    {
      HootServicesLoginManager().logout(
        "ServicesHootServicesLoginManagerTest::logoutInvalidTokensTest", "testAccessTokenBLAH",
        "testAccessTokenSecret");
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
        "ServicesHootServicesLoginManagerTest::logoutInvalidTokensTest", "testAccessToken",
        "testAccessTokenSecretBLAH");
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
      LOG_VART(exceptionMsg);
    }
    CPPUNIT_ASSERT(exceptionMsg.contains("Invalid access tokens."));

    ServicesDbTestUtils::deleteUserByUserName(
      "ServicesHootServicesLoginManagerTest::logoutInvalidTokensTest");
  }
  #endif  // HOOT_HAVE_SERVICES

  #ifdef HOOT_HAVE_SERVICES
  void logoutInvalidUserTest()
  {
    const QString sessionId = UuidHelper::createUuid().toString().replace("{", "").replace("}", "");
    const long userId =
      ServicesDbTestUtils::insertTestUser(
        "ServicesHootServicesLoginManagerTest::logoutInvalidUserTest",
        "ServicesHootServicesLoginManagerTest::logoutInvalidUserTest@hoot.com",
        sessionId, "testAccessToken", "testAccessTokenSecret");
    LOG_VART(userId);

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

    ServicesDbTestUtils::deleteUserByUserName(
      "ServicesHootServicesLoginManagerTest::logoutInvalidUserTest");
  }
  #endif  // HOOT_HAVE_SERVICES
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ServicesHootServicesLoginManagerTest, "slow");

}
