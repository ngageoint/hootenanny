
#include "HootNetworkCookieJar.h"

// Hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/io/HootApiDb.h>

namespace hoot
{

HootNetworkCookieJar::HootNetworkCookieJar(QObject* parent) :
QNetworkCookieJar(parent)
{
}

QString HootNetworkCookieJar::toString() const
{
  QString str;
  const QList<QNetworkCookie> cookies = allCookies();
  LOG_VART(cookies.size());
  for (QList<QNetworkCookie>::const_iterator itr = cookies.begin(); itr != cookies.end(); ++itr)
  {
    QNetworkCookie cookie = *itr;
    str += "Name: " + cookie.name() + ", Value: " + cookie.value() + "\n";
  }
  return str;
}

}
