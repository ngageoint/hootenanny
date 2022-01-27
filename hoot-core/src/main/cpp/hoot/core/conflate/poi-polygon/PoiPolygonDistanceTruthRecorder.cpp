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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "PoiPolygonDistanceTruthRecorder.h"

// hoot
#include <hoot/core/schema/MetadataTags.h>

using namespace std;

namespace hoot
{

QMultiMap<QString, double> PoiPolygonDistanceTruthRecorder::_poiMatchRefIdsToDistances;
QMultiMap<QString, double> PoiPolygonDistanceTruthRecorder::_polyMatchRefIdsToDistances;
QMultiMap<QString, double> PoiPolygonDistanceTruthRecorder::_poiReviewRefIdsToDistances;
QMultiMap<QString, double> PoiPolygonDistanceTruthRecorder::_polyReviewRefIdsToDistances;

void PoiPolygonDistanceTruthRecorder::recordDistanceTruth(ConstElementPtr poi, ConstElementPtr poly,
                                                          const QString& poiBestKvp, const QString& polyBestKvp,
                                                          const double elementDistance)
{
  QString infoStr =
    "Recording distance truth for: " + poi->getElementId().toString() + " and " +
    poly->getElementId().toString() +", POI KVP: " + poiBestKvp + ", poly kvp: " + polyBestKvp +
    ", distance: " + QString::number(elementDistance) + "...";

  // output feature distances for all feature types which fell within the match threshold
  const QString ref2 = poi->getTags().get(MetadataTags::Ref2());
  const QString review = poi->getTags().get("REVIEW");
  if (ref2 == poly->getTags().get(MetadataTags::Ref1()).split(";")[0])
  {
    infoStr = infoStr.replace("Recording distance", "Recording match distance");
    LOG_TRACE(infoStr);
    _poiMatchRefIdsToDistances.insert(poiBestKvp, elementDistance);
    _polyMatchRefIdsToDistances.insert(polyBestKvp, elementDistance);
  }
  else if (review == poly->getTags().get(MetadataTags::Ref1()).split(";")[0])
  {
    infoStr = infoStr.replace("Recording distance", "Recording review distance");
    LOG_TRACE(infoStr);
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

QString PoiPolygonDistanceTruthRecorder::getMatchDistanceInfo()
{
  LOG_VART(_poiMatchRefIdsToDistances.size());
  LOG_VART(_polyMatchRefIdsToDistances.size());
  LOG_VART(_poiReviewRefIdsToDistances.size());
  LOG_VART(_polyReviewRefIdsToDistances.size());

  QString info;
  info += _getMatchDistanceInfo("POI Match", _poiMatchRefIdsToDistances);
  info += _getMatchDistanceInfo("Poly Match", _polyMatchRefIdsToDistances);
  info += _getMatchDistanceInfo("POI Review", _poiReviewRefIdsToDistances);
  info += _getMatchDistanceInfo("Poly Review", _polyReviewRefIdsToDistances);
  return info;
}

QString PoiPolygonDistanceTruthRecorder::_getMatchDistanceInfo(const QString& matchType,
                                                               const QMultiMap<QString, double>& distanceInfo)
{
  if (distanceInfo.isEmpty())
    return matchType.toUpper() + ": no matches\n";

  QString info;
  for (const auto& type : distanceInfo.uniqueKeys())
  {
    LOG_VART(type);
    if (!type.trimmed().isEmpty())
    {
      double maxDistance = 0.0;
      double minimumDistance = 99999;
      double averageDistance = 0.0;
      int numberOfEntries = 0;

      QList<double> distances = distanceInfo.values(type);
      qSort(distances.begin(), distances.end());
      LOG_VART(distances);
      double sumDist = 0.0;
      QString distancesStr = "";
      for (auto dist : qAsConst(distances))
      {
        maxDistance = max(maxDistance, dist);
        minimumDistance = min(minimumDistance, dist);
        sumDist += dist;
        distancesStr += QString::number(dist) + ", ";
        numberOfEntries++;
      }
      distancesStr.chop(2);
      LOG_VART(maxDistance);
      LOG_VART(minimumDistance);
      averageDistance = sumDist / (double)distances.size();
      LOG_VART(averageDistance);

      info += matchType.toUpper() + " distance info for type: " + type + "\n";
      info += "Maximum distance: " + QString::number(maxDistance) + "\n";
      info += "Minimum distance: " + QString::number(minimumDistance) + "\n";
      info += "Average distance: " + QString::number(averageDistance) + "\n";
      info += "Number of matches: " + QString::number(numberOfEntries) + "\n";
      info += "Distances: " + distancesStr + "\n";
      info += "**************************\n";
      LOG_VART(info);
    }
  }
  return info;
}

}

