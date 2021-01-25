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
 * @copyright Copyright (C) 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef INTERSECTION_OVER_UNION_EXTRACTOR_H
#define INTERSECTION_OVER_UNION_EXTRACTOR_H

// hoot
#include <hoot/core/elements/Element.h>
#include <hoot/core/algorithms/extractors/FeatureExtractorBase.h>

namespace hoot
{

class OsmMap;

/**
 * Calculates Intersection over Union (IoU) for polygons
 *
 * https://stackoverflow.com/questions/28723670/intersection-over-union-between-two-detections
 *
 * https://www.pyimagesearch.com/2016/11/07/intersection-over-union-iou-for-object-detection/
 */
class IntersectionOverUnionExtractor : public FeatureExtractorBase
{
public:

  static std::string className() { return "hoot::IntersectionOverUnionExtractor"; }

  IntersectionOverUnionExtractor() = default;
  virtual ~IntersectionOverUnionExtractor() = default;

  /**
   * @see FeatureExtractor
   */
  virtual QString getName() const override { return QString::fromStdString(className()); }

  /**
   * @see FeatureExtractor
   */
  virtual double extract(const OsmMap& map, const ConstElementPtr& target,
    const ConstElementPtr& candidate) const override;

  /**
   * @see ApiEntityInfo
   */
  virtual QString getDescription() const
  { return "Calculates Intersection over Union (IoU) for polygons"; }
};

}

#endif // INTERSECTION_OVER_UNION_EXTRACTOR_H
