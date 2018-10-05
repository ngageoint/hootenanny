
// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/io/HootNetworkRequest.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/io/HootApiDb.h>
//#include <hoot/core/util/NetworkUtils.h>

// Std
#include <iostream>
#include <string>

// Qt
#include <QNetworkCookieJar>

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
    // auth through the 3rd party (OpenStreetMap, MapEdit, etc.)
    // verify the user
    // get the access tokens and show to the user
    _printAccessTokens(_verifyUser(_getRequestToken(), _getAuthorizationVerifier()));

    return 0;
  }

private:

  //boost::shared_ptr<HootNetworkCookieJar> _cookies;
  boost::shared_ptr<QNetworkCookieJar> _cookies;

  QString _getRequestToken()
  {
    HootNetworkRequest requestTokenRequest;
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
    // will need this session cookie for verification to be successful
    //TODO: print out cookies
    _cookies = requestTokenRequest.getCookies();
    //LOG_VART(_cookies->size());
    //LOG_VART(_cookies->toString());
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
    //LOG_VART(_cookies->size());
    //LOG_VART(_cookies->toString());
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
    _cookies = verifyRequest.getCookies();
    //LOG_VART(_cookies->size());
    //LOG_VART(_cookies->toString());

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
    db.open(HootApiDb::getBaseUrl());
    const QString accessToken = db.getAccessTokenByUserId(userId);
    LOG_VARD(accessToken);
    const QString accessTokenSecret = db.getAccessTokenSecretByUserId(userId);
    LOG_VARD(accessTokenSecret);
    std::cout << "oauth_token=" << accessToken << std::endl;
    std::cout << "oauth_token_secret=" << accessTokenSecret << std::endl;
  }
};

HOOT_FACTORY_REGISTER(Command, LoginCmd)

}

