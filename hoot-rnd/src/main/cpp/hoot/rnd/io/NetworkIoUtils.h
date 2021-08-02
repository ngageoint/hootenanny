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

#ifndef NETWORK_IO_UTILS_H
#define NETWORK_IO_UTILS_H

// Hoot
#include <hoot/core/io/HootNetworkCookieJar.h>

namespace hoot
{

/**
 * Various utilities for use in network programming
 */
class NetworkIoUtils
{

public:

  /**
   * Retrieves a session cookie that can be associated with a user account
   *
   * @param userName user name of the user
   * @param accessToken the user's public OAuth access token
   * @param accessTokenSecret the user's private OAuth access token
   * @param url URL of the request containing the cookies
   * @return a cookie store containing only the session cookie
   */
  static std::shared_ptr<HootNetworkCookieJar> getUserSessionCookie(
    const QString& userName, const QString& accessToken, const QString& accessTokenSecret,
    const QString& url);
};

}

#endif // NETWORK_IO_UTILS_H
