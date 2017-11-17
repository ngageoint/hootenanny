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
#include "WayFeatureExtractor.h"

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/algorithms/aggregator/MeanAggregator.h>

using namespace std;

namespace hoot
{

WayFeatureExtractor::WayFeatureExtractor(ValueAggregatorPtr agg):
  _agg(agg)
{
  if (!_agg)
    _agg.reset(new MeanAggregator());
}

WayFeatureExtractor::WayFeatureExtractor()
{
  _agg.reset(new MeanAggregator());
}

double WayFeatureExtractor::extract(const OsmMap& map,
  const boost::shared_ptr<const Element>& target, const boost::shared_ptr<const Element>& candidate) const
{
  vector<double> scores;

  if (target->getElementType() == ElementType::Way &&
      candidate->getElementType() == ElementType::Way)
  {
    scores.push_back(_extract(map, boost::dynamic_pointer_cast<const Way>(target),
                              boost::dynamic_pointer_cast<const Way>(candidate)));
  }
  else if (target->getElementType() == ElementType::Relation &&
           candidate->getElementType() == ElementType::Relation)
  {
    ConstRelationPtr r1 = boost::dynamic_pointer_cast<const Relation>(target);
    ConstRelationPtr r2 = boost::dynamic_pointer_cast<const Relation>(candidate);

    if (r1->getType() == MetadataTags::RelationMultilineString() &&
        r2->getType() == MetadataTags::RelationMultilineString() &&
        r1->getMembers().size() == r2->getMembers().size())
    {
      for (size_t i = 0; i < r1->getMembers().size(); i++)
      {
        ElementId eid1 = r1->getMembers()[i].getElementId();
        ElementId eid2 = r2->getMembers()[i].getElementId();

        if (eid1.getType() != ElementType::Way || eid2.getType() != ElementType::Way)
        {
          return nullValue();
        }
        scores.push_back(_extract(map, map.getWay(eid1), map.getWay(eid2)));
      }
    }
    else
    {
      return nullValue();
    }
  }
  else
  {
    return nullValue();
  }
  return _agg->aggregate(scores);
}

string WayFeatureExtractor::getName() const
{
  return getClassName() + (" agg: " + _agg->toString()).toStdString();
}

}
