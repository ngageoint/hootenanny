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
 * @copyright Copyright (C) 2023 Maxar (http://www.maxar.com/)
 */

#include "ApiElementRetrievalInterface.h"

//  Qt
#include <QRegularExpression>

//  Hootenanny
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/OsmApiUtils.h>

namespace hoot
{

ApiElementRetrievalInterface::ApiElementRetrievalInterface()
  : _isOverpassQuery(false)
{
}

bool ApiElementRetrievalInterface::hasMoreQueries() const
{
  return !_missingElements.empty() && _current != _missingElements.end();
}

QString ApiElementRetrievalInterface::getNextQuery(const QString& baseUrl)
{
  QString query;
  //  Check that there are elements
  if (!hasMoreQueries())
    return query;
  //  Check
  QString url = baseUrl;
  bool isOverpass = baseUrl.endsWith(OsmApiEndpoints::OVERPASS_API_PATH);
  bool isOsmApi = baseUrl.endsWith(OsmApiEndpoints::OSM_API_PATH_MAP);
  if (isOverpass)
  {
    //  Create an overpass query
    query = url + QString("?data=[out:xml];(%1(id:%2);(._;>;););out body;").arg(_current->getType().toString().toLower()).arg(_current->getId());
  }
  else if (isOsmApi)
  {
    //  Create a query to download the full element (including nodes for ways or all members of relations)
    query = url.replace(OsmApiEndpoints::OSM_API_PATH_MAP, "") +
            QString(OsmApiEndpoints::OSM_API_PATH_GET_ELEMENT_FULL).arg(_current->getType().toString().toLower()).arg(_current->getId());
  }
  else
  {
    LOG_WARN("Unknown OSM API: " << baseUrl);
  }
  ++_current;
  return query;
}

void ApiElementRetrievalInterface::setMissingElements(const std::set<ElementId>& elements)
{
  _missingElements = elements;
  _current = _missingElements.begin();
}

}
