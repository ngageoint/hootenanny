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
#ifndef POIPOLYGONMATCHDISTANCECALCULATOR_H
#define POIPOLYGONMATCHDISTANCECALCULATOR_H

// Hoot
#include <hoot/core/elements/Element.h>

// Qt
#include <QString>

namespace hoot
{

/**
 *
 */
class PoiPolygonMatchDistanceCalculator
{

public:

  PoiPolygonMatchDistanceCalculator(double matchDistanceDefault, double reviewDistanceDefault,
                                    const Tags& polyTags, long searchRadius,
                                    long surroundingPolyCount);

  double getMatchDistanceForType(const QString typeKvp) const;
  double getReviewDistanceForType(const QString typeKvp) const;
  void modifyReviewDistanceForPolyDensity(double& distance);
  void modifyMatchDistanceForPolyDensity(double& distance);

private:

  double _matchDistanceDefault;
  double _reviewDistanceDefault;

  Tags _polyTags;
  long _searchRadius;
  long _surroundingPolyCount;

  QString _testUuid;

  double _getPolyDensity() const;

};

}

#endif // POIPOLYGONMATCHDISTANCECALCULATOR_H
