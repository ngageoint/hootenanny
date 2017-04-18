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
#ifndef BUFFEREDOVERLAPEXTRACTOR_H
#define BUFFEREDOVERLAPEXTRACTOR_H

// hoot
#include <hoot/core/conflate/extractors/FeatureExtractor.h>

namespace hoot
{

class Element;

/**
 * Shamelessly taken from JCS.
 *
 * This is exactly the same as the SymDiffMatcher (in JCS speak) so I've skipped implementing the
 * SymDiffMatcher.
 *
 * @author RoadMatcher
 * @copyright GPL
 * http://www.vividsolutions.com/products.asp?catg=spaapp&code=roadmatcher
 * The ideas were shamelessly taken from RoadMatcher, but reimplemented in C++ with Hootenanny
 * appropriate data structures.
 */
class BufferedOverlapExtractor : public FeatureExtractor
{
public:
  /**
   * Similar to OverlapExtractor, but the objects are buffered before the overlap is calculated.
   *
   * @param bufferPortion Buffer the objects by this portion of the sqrt of the max area.
   */
  BufferedOverlapExtractor(double bufferPortion=0.1);

  static string className() { return "hoot::BufferedOverlapExtractor"; }

  virtual string getClassName() const { return BufferedOverlapExtractor::className(); }

  virtual string getName() const;

  virtual DataFrame::FactorType getFactorType() const { return DataFrame::Numerical; }

  virtual DataFrame::NullTreatment getNullTreatment() const
  {
    return DataFrame::NullAsMissingValue;
  }

  virtual double extract(const OsmMap& map, const boost::shared_ptr<const Element>& target,
    const boost::shared_ptr<const Element>& candidate) const;

private:
  double _bufferPortion;
};

}

#endif // OVERLAPEXTRACTOR_H
