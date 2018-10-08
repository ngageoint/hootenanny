
#include "HootServicesLoginManager.h"

// Hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/io/HootNetworkRequest.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/io/HootApiDb.h>
#include <hoot/core/io/HootNetworkCookieJar.h>

// Std
#include <iostream>
#include <string>

// Boost
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

namespace hoot
{

HootServicesLoginManager::HootServicesLoginManager()
{
}

QString HootServicesLoginManager::getRequestToken()
{
  HootNetworkRequest requestTokenRequest;
  _cookies.reset(new HootNetworkCookieJar());
  requestTokenRequest.setCookies(_cookies);
  try
  {
    LOG_VART(ConfigOptions().getHootServicesAuthRequestTokenEndpoint());
    requestTokenRequest.networkRequest(ConfigOptions().getHootServicesAuthRequestTokenEndpoint());
  }
  catch (const std::exception& e)
  {
    const QString exceptionMsg = e.what();
    throw HootException("Error retrieving request token. error: " + exceptionMsg);
  }
  if (requestTokenRequest.getHttpStatus() != 200)
  {
    throw HootException(
      "Error retrieving request token. error: " + requestTokenRequest.getErrorString());
  }
  LOG_VART(_cookies->size());
  LOG_VART(_cookies->toString());
  const QUrl authUrl(requestTokenRequest.getResponseContent());
  const QString requestToken = authUrl.queryItemValue("oauth_token");
  LOG_VARD(requestToken);

  std::cout << std::endl << "Authorization URL: " << authUrl.toString() << std::endl << std::endl;

  return requestToken;
}

QString HootServicesLoginManager::promptForAuthorizationVerifier() const
{
  std::string consoleInput;
  std::cout <<
    "1. Using the authorization URL shown above, authenticate through the 3rd party " <<
    "application." << std::endl;
  std::cout << "2. Grant Hootenanny access to the application." << std::endl;
  std::cout << "3. Copy and paste your verifier code from the browser here and press ENTER: ";
  std::getline(std::cin, consoleInput);
  const QString verifier = QString::fromStdString(consoleInput);
  LOG_VARD(verifier);
  return verifier;
}

long HootServicesLoginManager::verifyUserAndLogin(const QString requestToken,
                                                  const QString verifier)
{
  HootNetworkRequest verifyRequest;
  LOG_VART(_cookies->size());
  LOG_VART(_cookies->toString());
  verifyRequest.setCookies(_cookies);
  LOG_VART(ConfigOptions().getHootServicesAuthVerifyEndpoint());
  QUrl verifyUrl(ConfigOptions().getHootServicesAuthVerifyEndpoint());
  verifyUrl.addQueryItem("oauth_token", QString(QUrl::toPercentEncoding(requestToken)));
  verifyUrl.addQueryItem("oauth_verifier", QString(QUrl::toPercentEncoding(verifier)));
  LOG_VART(verifyUrl.toString());
  try
  {
    verifyRequest.networkRequest(verifyUrl.toString());
  }
  catch (const std::exception& e)
  {
    const QString exceptionMsg = e.what();
    throw HootException("Error verifying user. error: " + exceptionMsg);
  }
  if (verifyRequest.getHttpStatus() != 200)
  {
    throw HootException("Error verifying user. error: " + verifyRequest.getErrorString());
  }
  LOG_VART(_cookies->size());
  LOG_VART(_cookies->toString());

  // reply contains a user object
  boost::shared_ptr<boost::property_tree::ptree> replyObj =
    StringUtils::jsonStringToPropTree(verifyRequest.getResponseContent());
  const long userId = replyObj->get<long>("id");
  LOG_VARD(userId);
  return userId;
}

void HootServicesLoginManager::printAccessTokens(const long userId)
{
  HootApiDb db;
  LOG_VARD(HootApiDb::getBaseUrl());
  //hoot db requires a layer to open, but we don't need one here...so put anything in
  QUrl url(HootApiDb::getBaseUrl().toString() + "/blah");
  db.open(url);
  const QString accessToken = db.getAccessTokenByUserId(userId);
  LOG_VARD(accessToken);
  const QString accessTokenSecret = db.getAccessTokenSecretByUserId(userId);
  LOG_VARD(accessTokenSecret);
  db.close();

  std::cout << std::endl;
  std::cout << "oauth_token=" << accessToken << std::endl;
  std::cout << "oauth_token_secret=" << accessTokenSecret << std::endl;
}

bool HootServicesLoginManager::logout(const QString userName, const QString accessToken,
                                      const QString accessTokenSecret)
{
  HootApiDb db;
  //hoot db requires a layer to open, but we don't need one here...so put anything in
  QUrl url(HootApiDb::getBaseUrl().toString() + "/blah");
  db.open(url);
  if (!db.accessTokensAreValid(userName, accessToken, accessTokenSecret))
  {
    throw HootException("Unable to log out user: " + userName + ".  Invalid access tokens.");
  }

  // log the user out
  HootNetworkRequest logoutRequest;
  try
  {
    logoutRequest.networkRequest(ConfigOptions().getHootServicesAuthLogoutEndpoint());
  }
  catch (const std::exception& e)
  {
    throw HootException("Error logging out user: " + userName + ". error: " + e.what());
  }
  if (logoutRequest.getHttpStatus() != 200)
  {
    return false;
  }
  return true;
}

}
