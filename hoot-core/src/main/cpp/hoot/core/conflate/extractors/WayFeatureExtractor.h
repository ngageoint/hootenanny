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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef WAYFEATUREEXTRACTOR_H
#define WAYFEATUREEXTRACTOR_H

// hoot
#include <hoot/core/algorithms/aggregator/ValueAggregatorConsumer.h>
#include <hoot/core/elements/Way.h>

#include "FeatureExtractorBase.h"

namespace hoot
{

class OsmMap;

/**
 * Extracts a feature from two ways. If the features are not both ways and they aren't both
 * multilinestrings of the same count then a null is returned. If they're both multilinestrings then
 * it is assumed that the first ways in the multilinestring correspond along with the second, etc.
 */
class WayFeatureExtractor : public FeatureExtractorBase, public ValueAggregatorConsumer
{
public:
  static string className() { return "hoot::WayFeatureExtractor"; }

  WayFeatureExtractor(ValueAggregator* agg = 0);

  virtual double extract(const OsmMap& map, const boost::shared_ptr<const Element>& target,
    const boost::shared_ptr<const Element>& candidate) const;

  virtual string getClassName() const { return className(); }

  virtual string getName() const;

  virtual void setValueAggregator(const ValueAggregatorPtr& va) { _agg = va; }

protected:
  virtual double _extract(const OsmMap& map, const ConstWayPtr& w1, const ConstWayPtr& w2)
    const = 0;

  ValueAggregatorPtr _agg;
};

}

#endif // WAYFEATUREEXTRACTOR_H
