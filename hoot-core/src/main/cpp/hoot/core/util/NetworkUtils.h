
#ifndef NETWORK_UTILS_H
#define NETWORK_UTILS_H

// Hoot
#include <hoot/core/io/HootNetworkCookieJar.h>

// Qt
#include <QNetworkCookieJar>

namespace hoot
{

/**
 *
 */
class NetworkUtils
{

public:

   /**
   * @brief getUserSessionCookie
   * @param userName
   * @param accessToken
   * @param accessTokenSecret
   * @param url
   * @return
   */
  static boost::shared_ptr<HootNetworkCookieJar> getUserSessionCookie(
    const QString userName, const QString accessToken, const QString accessTokenSecret,
    const QString url);
};

}

#endif // NETWORK_UTILS_H
