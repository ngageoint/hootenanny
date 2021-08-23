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

#include "HootServicesLoginManager.h"

// Hoot
#include <hoot/core/io/HootApiDb.h>
#include <hoot/core/io/HootNetworkCookieJar.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/HootNetworkUtils.h>

#include <hoot/core/util/StringUtils.h>

// Std



// Boost
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

// Qt
#include <QUrl>
#include <QUrlQuery>

namespace hoot
{

HootServicesLoginManager::HootServicesLoginManager()
  : _timeout(500)
{
}

QString HootServicesLoginManager::getBaseUrl()
{
  ConfigOptions opts = ConfigOptions(conf());
  //assuming for now we don't need to go over https
  QString url = "http://" + opts.getHootServicesAuthHost();
  const int port = opts.getHootServicesAuthPort();
  if (port != 80)
  {
    url += ":" + QString::number(port);
  }
  return url + "/hoot-services";
}

QString HootServicesLoginManager::_getRequestTokenUrl()
{
  return getBaseUrl() + "/auth/oauth1/request";
}

QString HootServicesLoginManager::_getVerifyUrl()
{
  return getBaseUrl() + "/auth/oauth1/verify";
}

QString HootServicesLoginManager::_getLogoutUrl()
{
  return getBaseUrl() + "/auth/oauth1/logout";
}

QString HootServicesLoginManager::getRequestToken(QString& authUrlStr)
{
  HootNetworkRequest requestTokenRequest;
  _cookies = std::make_shared<HootNetworkCookieJar>();
  requestTokenRequest.setCookies(_cookies);
  try
  {
    LOG_VART(_getRequestTokenUrl());
    requestTokenRequest.networkRequest(_getRequestTokenUrl(), _timeout);
  }
  catch (const HootException& e)
  {
    const QString exceptionMsg = e.what();
    throw HootException("Error retrieving request token. error: " + exceptionMsg);
  }
  if (requestTokenRequest.getHttpStatus() != HttpResponseCode::HTTP_OK)
  {
    throw HootException(
      "Error retrieving request token. error: " + requestTokenRequest.getErrorString());
  }
  LOG_VART(_cookies->size());
  LOG_VART(_cookies->toString());
  const QUrl authUrl(requestTokenRequest.getResponseContent());
  const QString requestToken = QUrlQuery(authUrl).queryItemValue("oauth_token");
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

long HootServicesLoginManager::verifyUserAndLogin(const QString& requestToken,
                                                  const QString& verifier, QString& userName) const
{
  QUrl loginUrl;
  HootNetworkRequest loginRequest = _getLoginRequest(requestToken, verifier, loginUrl);
  try
  {
    loginRequest.networkRequest(loginUrl.toString(), _timeout);
  }
  catch (const HootException& e)
  {
    const QString exceptionMsg = e.what();
    throw HootException("Error verifying user. error: " + exceptionMsg);
  }
  if (loginRequest.getHttpStatus() != HttpResponseCode::HTTP_OK)
  {
    throw HootException("Error verifying user. error: " + loginRequest.getErrorString());
  }
  LOG_VART(_cookies->size());
  LOG_VART(_cookies->toString());

  // reply contains a user object
  const long userId = _parseLoginResponse(loginRequest.getResponseContent());

  HootApiDb db;
  //hoot db requires a layer to open, but we don't need one here...so put anything in
  QUrl url(HootApiDb::getBaseUrl().toString() + "/blah");
  db.open(url);
  userName = db.getUserNameById(userId);

  return userId;
}

HootNetworkRequest HootServicesLoginManager::_getLoginRequest(const QString& requestToken,
                                                              const QString& verifier,
                                                              QUrl& loginUrl) const
{
  HootNetworkRequest loginRequest;
  LOG_VART(_cookies->size());
  LOG_VART(_cookies->toString());
  loginRequest.setCookies(_cookies);
  LOG_VART(_getVerifyUrl());

  loginUrl.setUrl(_getVerifyUrl());
  QUrlQuery loginUrlQuery(loginUrl);
  loginUrlQuery.addQueryItem("oauth_token", QString(QUrl::toPercentEncoding(requestToken)));
  loginUrlQuery.addQueryItem("oauth_verifier", QString(QUrl::toPercentEncoding(verifier)));
  loginUrl.setQuery(loginUrlQuery);
  LOG_VART(loginUrl.toString());

  return loginRequest;
}

long HootServicesLoginManager::_parseLoginResponse(const QString& response) const
{
  LOG_VART(response);
  std::shared_ptr<boost::property_tree::ptree> replyObj =
    StringUtils::jsonStringToPropTree(response);
  const long userId = replyObj->get<long>("id");
  LOG_VARD(userId);
  return userId;
}

void HootServicesLoginManager::getAccessTokens(const long userId, QString& accessToken,
                                               QString& accessTokenSecret) const
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
  db.close();
}

bool HootServicesLoginManager::logout(const QString& userName, const QString& accessToken,
                                      const QString& accessTokenSecret) const
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
    logoutRequest.networkRequest(_getLogoutUrl(), _timeout);
  }
  catch (const HootException& e)
  {
    throw HootException("Error logging out user: " + userName + ". error: " + e.what());
  }
  if (logoutRequest.getHttpStatus() != HttpResponseCode::HTTP_OK)
  {
    return false;
  }

  // The services invalidate the http session associated with the user account on a logout but
  // don't remove the user account record.  If the user account record isn't removed here, then the
  // user really isn't logged out from a core perspective and this functionality has no purpose.
  // So, going to remove the user here.  For the longer term, it may be better to perform this
  // step, or something similar, in the services rather than right here.
  db.deleteUser(db.getUserIdByName(userName));

  return true;
}

}
