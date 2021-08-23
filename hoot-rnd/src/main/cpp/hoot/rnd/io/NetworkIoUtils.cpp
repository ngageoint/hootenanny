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
 * @copyright Copyright (C) 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */

#include "NetworkIoUtils.h"

// Hoot
#include <hoot/core/io/HootApiDb.h>
#include <hoot/core/util/HootException.h>


namespace hoot
{

std::shared_ptr<HootNetworkCookieJar> NetworkIoUtils::getUserSessionCookie(
  const QString& userName, const QString& accessToken, const QString& accessTokenSecret,
  const QString& url)
{
  LOG_VART(userName);
  LOG_VART(accessToken);
  LOG_VART(url);

  HootApiDb db;
  LOG_VART(HootApiDb::getBaseUrl());
  //hoot db requires a layer to open, but we don't need one here...so put anything in
  QUrl dbUrl(HootApiDb::getBaseUrl().toString() + "/blah");
  db.open(dbUrl);
  const QString sessionId = db.getSessionIdByAccessTokens(userName, accessToken, accessTokenSecret);
  LOG_VART(sessionId);
  db.close();
  if (sessionId.isEmpty())
  {
    throw HootException("User: " + userName + " has not been authenticated.");
  }

  std::shared_ptr<HootNetworkCookieJar> cookieJar = std::make_shared<HootNetworkCookieJar>();
  QList<QNetworkCookie> cookies;
  QNetworkCookie sessionCookie(QString("SESSION").toUtf8(), sessionId.toUtf8());
  cookies.append(sessionCookie);
  cookieJar->setCookiesFromUrl(cookies, url);
  LOG_VART(cookieJar->size());
  LOG_VART(cookieJar->toString());
  return cookieJar;
}

}
