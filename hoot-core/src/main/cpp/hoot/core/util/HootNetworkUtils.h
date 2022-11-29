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
 * @copyright Copyright (C) 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#ifndef HOOT_NETWORK_UTILS_H
#define HOOT_NETWORK_UTILS_H

namespace hoot
{

namespace HttpResponseCode
{
  const int HTTP_OK                     = 200;
  const int HTTP_BAD_REQUEST            = 400;
  const int HTTP_UNAUTHORIZED           = 401;
  const int HTTP_NOT_FOUND              = 404;
  const int HTTP_METHOD_NOT_ALLOWED     = 405;
  const int HTTP_CONFLICT               = 409;
  const int HTTP_GONE                   = 410;
  const int HTTP_PRECONDITION_FAILED    = 412;
  const int HTTP_TOO_MANY_REQUESTS      = 429;
  const int HTTP_INTERNAL_SERVER_ERROR  = 500;
  const int HTTP_BAD_GATEWAY            = 502;
  const int HTTP_SERVICE_UNAVAILABLE    = 503;
  const int HTTP_GATEWAY_TIMEOUT        = 504;
  const int HTTP_BANDWIDTH_EXCEEDED     = 509;
}

class HootNetworkUtils
{
public:
  static constexpr const char* CONTENT_TYPE_XML = "text/xml; charset=UTF-8";
  static constexpr const char* CONTENT_TYPE_JSON = "application/json; charset=UTF-8";
};

}

#endif  //  HOOT_NETWORK_UTILS_H
