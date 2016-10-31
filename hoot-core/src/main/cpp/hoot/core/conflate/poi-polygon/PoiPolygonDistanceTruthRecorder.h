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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef POIPOLYGONDISTANCETRUTHRECORDER_H
#define POIPOLYGONDISTANCETRUTHRECORDER_H

// hoot
#include <hoot/core/elements/Element.h>

// Qt
#include <QString>
#include <QMultiMap>

namespace hoot
{

/**
 * Records the actual distances between features involved in a manual match (both marked matches
 * and reviews).  This information can be used to derive custom match/review distances for the
 * additive model.  This logic is disabled by default.
 */
class PoiPolygonDistanceTruthRecorder
{

public:

  /**
   * Records distance information for the input feature pair
   *
   * @param poi the POI for which information is being recorded
   * @param poly the polygon for which information is being recorded
   * @param poiBestKvp the highest scoring type key value pair for the poi
   * @param polyBestKvp the highest scoring type key value pair for the poly
   * @param elementDistance the distance between the two elements
   */
  static void recordDistanceTruth(ConstElementPtr poi, ConstElementPtr poly,
                                  const QString poiBestKvp, const QString polyBestKvp,
                                  const double elementDistance);

  /**
   * Resets all recorded information
   */
  static void resetMatchDistanceInfo();

  /**
   * Prints all recorded information
   */
  static void printMatchDistanceInfo();

private:

  static QMultiMap<QString, double> _poiMatchRefIdsToDistances;
  static QMultiMap<QString, double> _poiReviewRefIdsToDistances;
  static QMultiMap<QString, double> _polyMatchRefIdsToDistances;
  static QMultiMap<QString, double> _polyReviewRefIdsToDistances;

  static void _printMatchDistanceInfo(const QString matchType,
                                      const QMultiMap<QString, double>& distanceInfo);

};

}

#endif // POIPOLYGONDISTANCETRUTHRECORDER_H
