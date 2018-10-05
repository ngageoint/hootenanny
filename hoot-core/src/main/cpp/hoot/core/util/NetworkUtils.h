
#ifndef NETWORK_UTILS_H
#define NETWORK_UTILS_H

// Qt
#include <QNetworkCookieJar>

namespace hoot
{

class HootNetworkCookieJar : public QNetworkCookieJar
{
public:

  HootNetworkCookieJar(QObject* parent = 0) :
  QNetworkCookieJar(parent)
  {
  }

  int size() const { return allCookies().size(); }

  QString toString() const
  {
    QString str;
    const QList<QNetworkCookie> cookies = allCookies();
    for (QList<QNetworkCookie>::const_iterator itr = cookies.begin(); itr != cookies.end(); ++itr)
    {
      QNetworkCookie cookie = *itr;
      str += "Name: " + cookie.name() + ", Value: " + cookie.value() + "\n";
    }
    return str;
  }
};

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
  static boost::shared_ptr<QNetworkCookieJar> getUserSessionCookie(
    const QString userName, const QString accessToken, const QString accessTokenSecret,
    const QString url);
};

}

#endif // NETWORK_UTILS_H
