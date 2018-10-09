
#ifndef HOOT_SERVICES_LOGIN_MANAGER_H
#define HOOT_SERVICES_LOGIN_MANAGER_H

// Hoot
#include <hoot/core/io/HootNetworkRequest.h>

// Qt
#include <QString>

namespace hoot
{

class HootNetworkCookieJar;

/**
 * TODO
 */
class HootServicesLoginManager
{
public:

  HootServicesLoginManager();

  /**
   * TODO
   *
   * @param authUrlStr
   * @return
   */
  QString getRequestToken(QString& authUrlStr);

  /**
   * TODO
   *
   * @return
   */
  QString promptForAuthorizationVerifier() const;

  /**
   * TODO
   *
   * @param requestToken
   * @param verifier
   * @return
   */
  long verifyUserAndLogin(const QString requestToken, const QString verifier);

  /**
   * TODO
   *
   * @param userId
   * @param accessToken
   * @param accessTokenSecret
   */
  void getAccessTokens(const long userId, QString& accessToken, QString& accessTokenSecret);

  /**
   * TODO
   *
   * @param userName
   * @param accessToken
   * @param accessTokenSecret
   * @return
   */
  bool logout(const QString userName, const QString accessToken, const QString accessTokenSecret);

private:

  friend class ServicesHootServicesLoginManagerTest;

  // hoot requires the same http session be used throughout the auth process, so the same session
  // cookie must be passed along with all OAuth requests
  boost::shared_ptr<HootNetworkCookieJar> _cookies;

  HootNetworkRequest _getLoginRequest(const QString requestToken, const QString verifier,
                                      QUrl& loginUrl);
  long _parseLoginResponse(const QString response);
};

}

#endif // HOOT_SERVICES_LOGIN_MANAGER_H
