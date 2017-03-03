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
#include "AttributeScoreExtractor.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/schema/TagComparator.h>
#include <hoot/core/util/ConfigOptions.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(FeatureExtractor, AttributeScoreExtractor)

AttributeScoreExtractor::AttributeScoreExtractor(ValueAggregator* wayAgg, bool useWeight) :
WayFeatureExtractor(wayAgg),
_useWeight(useWeight)
{
  setUseWeight(useWeight);
}

void AttributeScoreExtractor::setUseWeight(const bool useWeight)
{
  _useWeight = useWeight;
}

void AttributeScoreExtractor::setConfiguration(const Settings& conf)
{
  setUseWeight(ConfigOptions(conf).getAttributeScoreExtractorUseWeight());
}

double AttributeScoreExtractor::_extract(const OsmMap& /*map*/, const ConstWayPtr& w1,
                                         const ConstWayPtr& w2) const
{
  double score;
  double weight;
  TagComparator::getInstance().compareEnumeratedTags(w1->getTags(), w2->getTags(), score, weight);
  if (_useWeight)
  {
    return weight * score;
  }
  else
  {
    return score;
  }
}

string AttributeScoreExtractor::getName() const
{
  string result = getClassName() + (" agg: " + _agg->toString()).toStdString();
  if (_useWeight)
  {
    result += " use weight";
  }
  return result;
}

}
