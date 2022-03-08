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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef MAPCOMPARATOR_H
#define MAPCOMPARATOR_H

namespace hoot
{

class OsmMap;
class ElementType;

/**
 * Compares two maps.
 */
class MapComparator
{
public:

  MapComparator();

  /**
   * Returns true if the maps are essentially the same. Minor differences in node locations are
   * ignored.
   */
  bool isMatch(const std::shared_ptr<OsmMap>& ref, const std::shared_ptr<OsmMap>& test) const;

  void setIgnoreUUID() { _ignoreUUID = true; }
  void setUseDateTime() { _useDateTime = true; }
  void setErrorLimit(int limit) { _errorLimit = limit; }
  void setIgnoreTagKeys(const QStringList& keys) { _ignoreTagKeys = keys; }

private:

  bool _ignoreUUID;
  bool _useDateTime;
  int _errorLimit;
  QStringList _ignoreTagKeys;

  void _printIdDiff(const std::shared_ptr<OsmMap>& map1, const std::shared_ptr<OsmMap>& map2,
                    const ElementType& elementType, const int limit = 5) const;
};

}

#endif // MAPCOMPARATOR_H
