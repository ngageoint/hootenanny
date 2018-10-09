
#include "HootServicesLoginManager.h"

// Hoot
#include <hoot/core/util/Log.h>
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

QString HootServicesLoginManager::getRequestToken(QString& authUrlStr)
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
  authUrlStr = authUrl.toString();

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
  QUrl loginUrl;
  HootNetworkRequest loginRequest = _getLoginRequest(requestToken, verifier, loginUrl);
  try
  {
    loginRequest.networkRequest(loginUrl.toString());
  }
  catch (const std::exception& e)
  {
    const QString exceptionMsg = e.what();
    throw HootException("Error verifying user. error: " + exceptionMsg);
  }
  if (loginRequest.getHttpStatus() != 200)
  {
    throw HootException("Error verifying user. error: " + loginRequest.getErrorString());
  }
  LOG_VART(_cookies->size());
  LOG_VART(_cookies->toString());

  // reply contains a user object
  return _parseLoginResponse(loginRequest.getResponseContent());
}

HootNetworkRequest HootServicesLoginManager::_getLoginRequest(const QString requestToken,
                                                              const QString verifier,
                                                              QUrl& loginUrl)
{
  HootNetworkRequest loginRequest;
  LOG_VART(_cookies->size());
  LOG_VART(_cookies->toString());
  loginRequest.setCookies(_cookies);
  LOG_VART(ConfigOptions().getHootServicesAuthVerifyEndpoint());

  loginUrl.setUrl(ConfigOptions().getHootServicesAuthVerifyEndpoint());
  loginUrl.addQueryItem("oauth_token", QString(QUrl::toPercentEncoding(requestToken)));
  loginUrl.addQueryItem("oauth_verifier", QString(QUrl::toPercentEncoding(verifier)));
  LOG_VART(loginUrl.toString());

  return loginRequest;
}

long HootServicesLoginManager::_parseLoginResponse(const QString response)
{
  LOG_VART(response);
  boost::shared_ptr<boost::property_tree::ptree> replyObj =
    StringUtils::jsonStringToPropTree(response);
  const long userId = replyObj->get<long>("id");
  LOG_VARD(userId);
  return userId;
}

void HootServicesLoginManager::getAccessTokens(const long userId, QString& accessToken,
                                               QString& accessTokenSecret)
{
  HootApiDb db;
  LOG_VARD(HootApiDb::getBaseUrl());
  //hoot db requires a layer to open, but we don't need one here...so put anything in
  QUrl url(HootApiDb::getBaseUrl().toString() + "/blah");
  db.open(url);

  LOG_VART(db.userExists(userId));
  if (!db.userExists(userId))
  {
    throw HootException("User does not exist. ID: " + QString::number(userId));
  }

  accessToken = db.getAccessTokenByUserId(userId);
  LOG_VARD(accessToken);
  accessTokenSecret = db.getAccessTokenSecretByUserId(userId);
  LOG_VARD(accessTokenSecret);
  db.close();
}

bool HootServicesLoginManager::logout(const QString userName, const QString accessToken,
                                      const QString accessTokenSecret)
{
  HootApiDb db;
  //hoot db requires a layer to open, but we don't need one here...so put anything in
  QUrl url(HootApiDb::getBaseUrl().toString() + "/blah");
  db.open(url);

  if (!db.userExists(userName))
  {
    throw HootException("User does not exist. user name:" + userName);
  }
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

  // The services invalidate the http session associated with the user account but don't seem to
  // be invalidating the user account.  For the meantime, going to remove the user here so that
  // calling this log out actually does something.  For the long term, it would be better to do
  // this, or something similar, in the services instead and remove this delete.
  db.deleteUser(db.getUserIdByName(userName));

  return true;
}

}
