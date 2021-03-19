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
 * @copyright Copyright (C) 2005 VividSolutions (http://www.vividsolutions.com/)
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef SMALLEROVERLAPEXTRACTOR_H
#define SMALLEROVERLAPEXTRACTOR_H

// hoot
#include <hoot/core/algorithms/extractors/FeatureExtractorBase.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{

class Element;

/**
 * Based on the JCS OverlapExtractor.
 *
 * This extractor uses the feature with more overlap to calculate the percentage overlap. For
 * instance if you have a small feature contained within a larger feature, the result will be 1.0.
 * If either feature has an area of zero, then zero is returned.
 *
 * @author RoadMatcher
 * @copyright GPL
 * http://www.vividsolutions.com/products.asp?catg=spaapp&code=roadmatcher
 * The ideas were shamelessly taken from RoadMatcher, but reimplemented in C++ with Hootenanny
 * appropriate data structures.
 */
class SmallerOverlapExtractor : public FeatureExtractorBase, public Configurable
{
public:

  SmallerOverlapExtractor();
  ~SmallerOverlapExtractor() = default;

  static QString className() { return "hoot::SmallerOverlapExtractor"; }

  QString getClassName() const override { return className(); }

  double extract(const OsmMap& map, const std::shared_ptr<const Element>& target,
    const std::shared_ptr<const Element>& candidate) const override;

  QString getDescription() const override
  { return "Determines the overlap between two features focusing on the feature with more overlap"; }

  /**
   * @see Configurable
   */
  void setConfiguration(const Settings& conf) override;

  QString getName() const override { return className(); }

private:

  bool _requireAreaForPolygonConversion;
};

}

#endif // SMALLEROVERLAPEXTRACTOR_H
