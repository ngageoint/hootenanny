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
