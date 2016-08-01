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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef INTERSECTIONCRITERION_H
#define INTERSECTIONCRITERION_H

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/ConstOsmMapConsumer.h>
#include <hoot/core/filters/ElementCriterion.h>

namespace hoot
{

/**
 * This criterion is satisified for all highway intersections with 3 or more interacting ways.
 * Multilinestring relations are not handled.
 *
 * This class assumes that IntersectionSplitter was applied to the map before being called.
 */
class IntersectionCriterion : public ElementCriterion, public ConstOsmMapConsumer
{
public:
  static std::string className() { return "hoot::IntersectionCriterion"; }

  IntersectionCriterion() {}

  virtual ~IntersectionCriterion() {}

  IntersectionCriterion(ConstOsmMapPtr map);

  virtual ElementCriterion* clone() { return new IntersectionCriterion(_map); }

  virtual bool isSatisfied(const shared_ptr<const Element>& e) const;

  virtual void setOsmMap(const OsmMap* map);

private:
  set<long> _highwayIds;
  ConstOsmMapPtr _map;
};

}

#endif // INTERSECTIONCRITERION_H
