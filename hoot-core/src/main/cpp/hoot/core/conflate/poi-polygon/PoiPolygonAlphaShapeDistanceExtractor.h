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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef POIPOLYGONALPHASHAPEDISTANCEEXTRACTOR_H
#define POIPOLYGONALPHASHAPEDISTANCEEXTRACTOR_H

// hoot
#include <hoot/core/elements/Element.h>
#include <hoot/core/conflate/extractors/FeatureExtractorBase.h>


namespace hoot
{

/**
 * Calculates the convex hull distance between a poi and a polygon
 */
class PoiPolygonAlphaShapeDistanceExtractor : public FeatureExtractorBase
{
public:

  static string className() { return "hoot::PoiPolygonAlphaShapeDistanceExtractor"; }

  PoiPolygonAlphaShapeDistanceExtractor();

  virtual string getClassName() const
  { return PoiPolygonAlphaShapeDistanceExtractor::className(); }

  /**
   * Calculates the convex hull distance between a poi and a polygon
   *
   * @param map map containing the elements whose distance is to be determined
   * @param poi a POI element
   * @param poly a polygon element
   * @return the distance between the two elements
   */
  virtual double extract(const OsmMap& map, const ConstElementPtr& poi,
                         const ConstElementPtr& poly) const;

};

}

#endif // POIPOLYGONALPHASHAPEDISTANCEEXTRACTOR_H