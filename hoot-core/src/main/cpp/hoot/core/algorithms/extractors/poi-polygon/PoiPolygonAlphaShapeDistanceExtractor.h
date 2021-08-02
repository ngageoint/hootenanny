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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef POIPOLYGONALPHASHAPEDISTANCEEXTRACTOR_H
#define POIPOLYGONALPHASHAPEDISTANCEEXTRACTOR_H

// hoot
#include <hoot/core/elements/Element.h>
#include <hoot/core/algorithms/extractors/FeatureExtractorBase.h>


namespace hoot
{

/**
 * @brief The PoiPolygonAlphaShapeDistanceExtractor class calculates the convex hull distance
 * between a poi and a polygon for POI to Polygon conflation.
 */
class PoiPolygonAlphaShapeDistanceExtractor : public FeatureExtractorBase
{
public:

  static QString className() { return "hoot::PoiPolygonAlphaShapeDistanceExtractor"; }

  PoiPolygonAlphaShapeDistanceExtractor() = default;
  ~PoiPolygonAlphaShapeDistanceExtractor() = default;

  /**
   * Calculates the convex hull distance between a poi and a polygon
   *
   * @param map map containing the elements whose distance is to be determined
   * @param poi a POI element
   * @param poly a polygon element
   * @return the distance between the two elements
   */
  double extract(const OsmMap& map, const ConstElementPtr& poi,
                 const ConstElementPtr& poly) const override;

  QString getClassName() const override { return className(); }
  QString getDescription() const override
  { return "Calculates the convex hull distance between a poi and a polygon"; }
  QString getName() const override { return className(); }
};

}

#endif // POIPOLYGONALPHASHAPEDISTANCEEXTRACTOR_H
