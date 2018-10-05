
#ifndef HOOT_NETWORK_COOKIE_JAR_H
#define HOOT_NETWORK_COOKIE_JAR_H

// Qt
#include <QNetworkCookieJar>

namespace hoot
{

/**
 * This primarily exists to check the contents of the cookie store.  May not be needed with
 * Qt 5.
 */
class HootNetworkCookieJar : public QNetworkCookieJar
{
public:

  HootNetworkCookieJar(QObject* parent = 0);

  int size() const { return allCookies().size(); }

  QString toString() const;
};

}

#endif // HOOT_NETWORK_COOKIE_JAR_H
