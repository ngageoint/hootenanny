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
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef INTERSECTION_OVER_UNION_EXTRACTOR_H
#define INTERSECTION_OVER_UNION_EXTRACTOR_H

// geos
#include <geos/geom/Envelope.h>
#include <geos/geom/Point.h>

// hoot
#include <hoot/core/algorithms/extractors/FeatureExtractorBase.h>

namespace hoot
{

class Element;
class OsmMap;

/**
 * TODO
 */
class IntersectionOverUnionExtractor : public FeatureExtractorBase
{
public:

  static std::string className() { return "hoot::IntersectionOverUnionExtractor"; }

  virtual std::string getClassName() const override
  { return IntersectionOverUnionExtractor::className(); }

  virtual double extract(const OsmMap& map, const std::shared_ptr<const Element>& target,
    const std::shared_ptr<const Element>& candidate) const override;

  virtual QString getDescription() const { return "TODO"; }
};

}

#endif // INTERSECTION_OVER_UNION_EXTRACTOR_H
