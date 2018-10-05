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

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/cmd/BaseCommand.h>
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

class LoginCmd : public BaseCommand
{
public:

  static std::string className() { return "hoot::LoginCmd"; }

  LoginCmd() { }

  virtual QString getName() const { return "login"; }

  virtual QString getDescription() const
  { return "Logs a user into the Hootenanny Web Services"; }

  virtual int runSimple(QStringList args)
  {
    if (args.size() > 0)
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw HootException(QString("%1 takes zero parameters.").arg(getName()));
    }

    // get a request token
    const QString requestToken = _getRequestToken();

    // auth through the 3rd party (OpenStreetMap, MapEdit, etc.)
    const QString verifier = _getAuthorizationVerifier();

    // verify the user and get the user id
    const long userId = _verifyUser(requestToken, verifier);

    // get the access tokens and show to the user
    _printAccessTokens(userId);

    return 0;
  }

private:

  boost::shared_ptr<HootNetworkCookieJar> _cookies;

  QString _getRequestToken()
  {
    HootNetworkRequest requestTokenRequest;
    _cookies.reset(new HootNetworkCookieJar());
    requestTokenRequest.setCookies(_cookies);
    try
    {
      LOG_VART(ConfigOptions().getAuthRequestTokenEndpoint());
      requestTokenRequest.networkRequest(ConfigOptions().getAuthRequestTokenEndpoint());
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

    std::cout << "Authorization URL: " << authUrl.toString() << std::endl;

    return requestToken;
  }

  QString _getAuthorizationVerifier() const
  {
    std::string consoleInput;
    std::cout <<
      "Using the authorization URL shown above, authenticate through the 3rd party application, " <<
      "grant Hootenanny access, enter your verifier code here, and press ENTER: ";
    std::getline(std::cin, consoleInput);
    const QString verifier = QString::fromStdString(consoleInput);
    LOG_VARD(verifier);
    return verifier;
  }

  long _verifyUser(const QString requestToken, const QString verifier)
  {
    HootNetworkRequest verifyRequest;
    // hoot requires the same http session be used throughout the auth process, so a session cookie
    // must be passed here or the request token lookup during the token trade will fail
    LOG_VART(_cookies->size());
    LOG_VART(_cookies->toString());
    verifyRequest.setCookies(_cookies);
    LOG_VART(ConfigOptions().getAuthVerifyEndpoint());
    QUrl verifyUrl(ConfigOptions().getAuthVerifyEndpoint());
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

  void _printAccessTokens(const long userId)
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

    std::cout << "oauth_token=" << accessToken << std::endl;
    std::cout << "oauth_token_secret=" << accessTokenSecret << std::endl;
  }
};

HOOT_FACTORY_REGISTER(Command, LoginCmd)

}

