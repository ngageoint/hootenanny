
#include "NetworkUtils.h"

// Hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/io/HootApiDb.h>

namespace hoot
{

boost::shared_ptr<QNetworkCookieJar> NetworkUtils::getUserSessionCookie(
  const QString userName, const QString accessToken, const QString accessTokenSecret,
  const QString url)
{
  LOG_VART(userName);
  LOG_VART(accessToken);
  LOG_VART(accessTokenSecret);
  LOG_VART(url);

  HootApiDb db;
  db.open(HootApiDb::getBaseUrl());
  const QString sessionId = db.getSessionIdByAccessTokens(userName, accessToken, accessTokenSecret);
  LOG_VART(sessionId);
  if (sessionId.isEmpty())
  {
    throw HootException("User: " + userName + " has not been authenticated.");
  }

  //boost::shared_ptr<HootNetworkCookieJar> cookieJar(new HootNetworkCookieJar());
  boost::shared_ptr<QNetworkCookieJar> cookieJar(new QNetworkCookieJar());
  QList<QNetworkCookie> cookies;
  QNetworkCookie sessionCookie(QString("SESSION").toUtf8(), sessionId.toUtf8());
  cookies.append(sessionCookie);
  cookieJar->setCookiesFromUrl(cookies, url);
  //LOG_VART(cookieJar->size());
  //LOG_VART(cookieJar->toString());
  return cookieJar;
}

}
