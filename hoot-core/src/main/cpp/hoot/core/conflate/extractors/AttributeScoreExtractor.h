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
#ifndef ATTRIBUTESCOREEXTRACTOR_H
#define ATTRIBUTESCOREEXTRACTOR_H

#include "WayFeatureExtractor.h"

#include <hoot/core/util/Configurable.h>

namespace hoot
{

/**
 * See exporatory funds report for details.
 */
class AttributeScoreExtractor : public WayFeatureExtractor, public Configurable
{
public:
  static string className() { return "hoot::AttributeScoreExtractor"; }

  AttributeScoreExtractor(ValueAggregator* wayAgg = 0, bool useWeight = false);

  virtual string getClassName() const { return className(); }

  virtual string getName() const;

  virtual void setConfiguration(const Settings& conf);

  void setUseWeight(const bool useWeight);

protected:

  double _extract(const OsmMap& map, const ConstWayPtr& w1, const ConstWayPtr& w2) const;

  bool _useWeight;
};

}

#endif // ATTRIBUTESCOREEXTRACTOR_H
