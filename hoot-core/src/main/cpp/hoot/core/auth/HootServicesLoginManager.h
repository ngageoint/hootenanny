
#ifndef HOOT_SERVICES_LOGIN_MANAGER_H
#define HOOT_SERVICES_LOGIN_MANAGER_H

// Qt
#include <QString>

namespace hoot
{

class HootNetworkCookieJar;

/**
 *
 */
class HootServicesLoginManager
{
public:

  HootServicesLoginManager();

  /**
   * TODO
   *
   * @return
   */
  QString getRequestToken();

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
   */
  void printAccessTokens(const long userId);

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

  // hoot requires the same http session be used throughout the auth process, so the same session
  // cookie must be passed along with all OAuth requests
  boost::shared_ptr<HootNetworkCookieJar> _cookies;
};

}

#endif // HOOT_SERVICES_LOGIN_MANAGER_H
