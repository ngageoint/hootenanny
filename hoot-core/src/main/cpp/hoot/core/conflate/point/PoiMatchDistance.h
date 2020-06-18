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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef POI_MATCH_DISTANCE_H
#define POI_MATCH_DISTANCE_H

// Qt
#include <QList>

namespace hoot
{

/**
 * TODO
 */
class PoiMatchDistance
{
public:

  PoiMatchDistance(QString key, QString val, int maxMatchDistance, int maxReviewDistance);

  /**
   * Reads POI to POI conflation matches/review distances
   *
   * @param jsonStringOrFile path to a distances file (*.json) or a distances JSON string
   * @return a collection of match/review distances
   */
  static QList<PoiMatchDistance> readDistances(const QString& jsonStringOrFile);

  QString getKey() const { return _key; }
  QString getValue() const { return _value; }
  int getMaxMatchDistance() const { return _maxMatchDistance; }
  int getMaxReviewDistance() const { return _maxReviewDistance; }

private:

  QString _key;
  QString _value;
  int _maxMatchDistance;
  int _maxReviewDistance;
};

}

#endif // POI_MATCH_DISTANCE_H
