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
 * @copyright Copyright (C) 2005 VividSolutions (http://www.vividsolutions.com/)
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef OVERLAPEXTRACTOR_H
#define OVERLAPEXTRACTOR_H

// hoot
#include <hoot/core/conflate/extractors/FeatureExtractor.h>

namespace hoot
{

class Element;

/**
 * This is exactly the same as the SymDiffMatcher (in JCS speak) so I've skipped implementing the
 * SymDiffMatcher.
 *
 * All non-area geometries have 0% overlap. (due to 0 area)
 *
 * @author RoadMatcher
 * @copyright GPL
 * http://www.vividsolutions.com/products.asp?catg=spaapp&code=roadmatcher
 * The ideas were shamelessly taken from RoadMatcher, but reimplemented in C++ with Hootenanny
 * appropriate data structures.
 */
class OverlapExtractor : public FeatureExtractor
{
public:
  OverlapExtractor();

  static string className() { return "hoot::OverlapExtractor"; }

  virtual string getClassName() const { return OverlapExtractor::className(); }

  virtual DataFrame::FactorType getFactorType() const { return DataFrame::Numerical; }

  virtual DataFrame::NullTreatment getNullTreatment() const
  {
    return DataFrame::NullAsMissingValue;
  }

  virtual double extract(const OsmMap& map, const boost::shared_ptr<const Element>& target,
    const boost::shared_ptr<const Element>& candidate) const;
};

}

#endif // OVERLAPEXTRACTOR_H
