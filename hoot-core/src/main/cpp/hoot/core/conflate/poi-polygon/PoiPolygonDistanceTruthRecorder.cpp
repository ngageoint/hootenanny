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
#include "PoiPolygonDistanceTruthRecorder.h"

// hoot
#include <hoot/core/util/Log.h>


namespace hoot
{

QMultiMap<QString, double> PoiPolygonDistanceTruthRecorder::_poiMatchRefIdsToDistances;
QMultiMap<QString, double> PoiPolygonDistanceTruthRecorder::_polyMatchRefIdsToDistances;
QMultiMap<QString, double> PoiPolygonDistanceTruthRecorder::_poiReviewRefIdsToDistances;
QMultiMap<QString, double> PoiPolygonDistanceTruthRecorder::_polyReviewRefIdsToDistances;

void PoiPolygonDistanceTruthRecorder::recordDistanceTruth(ConstElementPtr poi,
                                                          ConstElementPtr poly,
                                                          const QString poiBestKvp,
                                                          const QString polyBestKvp,
                                                          const double elementDistance)
{
  //output feature distances for all feature types which fell within the match threshold
  const QString ref2 = poi->getTags().get("REF2");
  const QString review = poi->getTags().get("REVIEW");
  if (ref2 == poly->getTags().get("REF1").split(";")[0])
  {
    _poiMatchRefIdsToDistances.insert(poiBestKvp, elementDistance);
    _polyMatchRefIdsToDistances.insert(polyBestKvp, elementDistance);
  }
  else if (review == poly->getTags().get("REF1").split(";")[0])
  {
    _poiReviewRefIdsToDistances.insert(poiBestKvp, elementDistance);
    _polyReviewRefIdsToDistances.insert(polyBestKvp, elementDistance);
  }
}

void PoiPolygonDistanceTruthRecorder::resetMatchDistanceInfo()
{
  _poiMatchRefIdsToDistances.clear();
  _polyMatchRefIdsToDistances.clear();
  _poiReviewRefIdsToDistances.clear();
  _polyReviewRefIdsToDistances.clear();
}

void PoiPolygonDistanceTruthRecorder::printMatchDistanceInfo()
{
  _printMatchDistanceInfo("POI Match", _poiMatchRefIdsToDistances);
  _printMatchDistanceInfo("Poly Match", _polyMatchRefIdsToDistances);
  _printMatchDistanceInfo("POI Review", _poiReviewRefIdsToDistances);
  _printMatchDistanceInfo("Poly Review", _polyReviewRefIdsToDistances);
}


void PoiPolygonDistanceTruthRecorder::_printMatchDistanceInfo(const QString matchType,
  const QMultiMap<QString, double>& distanceInfo)
{
  foreach (QString type, distanceInfo.uniqueKeys())
  {
    if (!type.trimmed().isEmpty())
    {
      double maxDistance = 0.0;
      double minimumDistance = 99999;
      double averageDistance = 0.0;
      int numberOfEntries = 0;

      QList<double> distances = distanceInfo.values(type);
      qSort(distances.begin(), distances.end());
      double sumDist = 0.0;
      QString distancesStr = "";
      for (QList<double>::const_iterator itr = distances.begin(); itr != distances.end(); ++itr)
      {
        const double dist = *itr;
        maxDistance = max(maxDistance, dist);
        minimumDistance = min(minimumDistance, dist);
        sumDist += dist;
        distancesStr += QString::number(dist) + ", ";
        numberOfEntries++;
      }
      distancesStr.chop(2);
      averageDistance = sumDist / (double)distances.size();

      LOG_INFO(matchType.toUpper() << " distance info for type: " << type);
      LOG_VAR(maxDistance);
      LOG_VAR(minimumDistance);
      LOG_VAR(averageDistance);
      LOG_VAR(numberOfEntries);
      LOG_VAR(distancesStr);
      LOG_INFO("**************************");
    }
  }
}

}

