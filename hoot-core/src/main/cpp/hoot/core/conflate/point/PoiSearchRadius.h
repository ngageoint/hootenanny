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
 * @copyright Copyright (C) 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef POI_SEARCH_RADIUS_H
#define POI_SEARCH_RADIUS_H

// Qt
#include <QList>

namespace hoot
{

/**
 * POI to POI Conflation defines a maximum distance at which two features of the same type are
 * allowed to be compared. Separate distances are used for each tag kvp or key wildcard that POI to
 * POI supports. By default, these are defined in the file pointed to by the poi.search.radii.config
 * configuration option.
 */
class PoiSearchRadius
{
public:

  PoiSearchRadius(QString key, QString val, int distance);

  /**
   * Reads POI to POI conflation matches/review distances
   *
   * @param jsonStringOrFile path to a search radii file (*.json) or a search radii JSON string
   * @return a collection of search radii
   */
  static QList<PoiSearchRadius> readSearchRadii(const QString& jsonStringOrFile);

  QString getKey() const { return _key; }
  QString getValue() const { return _value; }
  int getDistance() const { return _distance; }

  QString toString() const;

private:

  QString _key;
  QString _value;
  int _distance;
};

}

#endif // POI_SEARCH_RADIUS_H
