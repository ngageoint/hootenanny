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
 * @copyright Copyright (C) 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef HOOT_SERVICES_LOGIN_MANAGER_H
#define HOOT_SERVICES_LOGIN_MANAGER_H

// Hoot
#include <hoot/core/io/HootNetworkRequest.h>

namespace hoot
{

class HootNetworkCookieJar;

/**
 * Manages user logins to the Hootenanny Web Services
 */
class HootServicesLoginManager
{
public:

  HootServicesLoginManager();

  /**
   * Retrieves an OAuth request token from the Hootenanny Web Services
   *
   * @param authUrlStr the 3rd party app authorization URL populated by the method to be used to
   * authorize Hootenanny's access to the login credentials
   * @return an OAuth request token
   */
  QString getRequestToken(QString& authUrlStr);

  /**
   * Prompts the user to enter the OAuth verification code after 3rd party authorization
   *
   * @return the OAuth verifier code input by the user
   */
  QString promptForAuthorizationVerifier() const;

  /**
   * Verifies a user's 3rd party authorization and logs them in to the Hootenanny Web Services
   *
   * @param requestToken an OAuth request token
   * @param verifier the OAuth verifier code associated with the input request token
   * @paraqm userName populated user name for the authenticated user
   * @return ID of the authenticated user
   */
  long verifyUserAndLogin(const QString& requestToken, const QString& verifier, QString& userName) const;

  /**
   * Retrieves OAuth access tokens for an authenticated user
   *
   * @param userId ID of the user
   * @param accessToken populated OAuth public access token
   * @param accessTokenSecret populated OAuth private access token
   */
  void getAccessTokens(const long userId, QString& accessToken, QString& accessTokenSecret) const;

  /**
   * Logs a user out of the Hootenanny Web Services
   *
   * @param userName user name of the user
   * @param accessToken the OAuth public access token for the user
   * @param accessTokenSecret the OAuth private access token for the user
   * @return true if the logout was successful; false otherwise
   */
  bool logout(const QString& userName, const QString& accessToken, const QString& accessTokenSecret) const;

  /**
   * Returns the base URL used to access Hootenanny Web Services
   *
   * @return a URL string
   */
  static QString getBaseUrl();

private:

  friend class ServicesHootServicesLoginManagerTest;

  // hoot requires the same http session be used throughout the auth process, so the same session
  // cookie must be passed along with all OAuth requests
  std::shared_ptr<HootNetworkCookieJar> _cookies;

  static QString _getRequestTokenUrl();
  static QString _getVerifyUrl();
  static QString _getLogoutUrl();

  HootNetworkRequest _getLoginRequest(const QString& requestToken, const QString& verifier,
                                      QUrl& loginUrl) const;
  long _parseLoginResponse(const QString& response) const;
  /** Timeout for HTTP requests in seconds */
  int _timeout;
};

}

#endif // HOOT_SERVICES_LOGIN_MANAGER_H
