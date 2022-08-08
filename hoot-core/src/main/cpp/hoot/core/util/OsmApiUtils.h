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

#ifndef OSM_API_UTILS_H
#define OSM_API_UTILS_H

namespace hoot
{

class OsmApiEndpoints
{
public:
  /** OSM API URL paths */
  static constexpr const char* API_PATH_MAP = "/api/0.6/map";
  static constexpr const char* API_PATH_CAPABILITIES = "/api/capabilities";
  static constexpr const char* API_PATH_PERMISSIONS = "/api/0.6/permissions";
  static constexpr const char* API_PATH_CREATE_CHANGESET = "/api/0.6/changeset/create";
  static constexpr const char* API_PATH_CLOSE_CHANGESET = "/api/0.6/changeset/%1/close";
  static constexpr const char* API_PATH_UPLOAD_CHANGESET = "/api/0.6/changeset/%1/upload";
  static constexpr const char* API_PATH_GET_ELEMENT = "/api/0.6/%1/%2";

};

}

#endif  //  OSM_API_UTILS_H
