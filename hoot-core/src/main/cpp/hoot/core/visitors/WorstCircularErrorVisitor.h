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

#ifndef WORSTCIRCULARERRORVISITOR_H
#define WORSTCIRCULARERRORVISITOR_H

#include <hoot/core/OsmMap.h>
#include <hoot/core/visitors/ElementConstOsmMapVisitor.h>
#include <hoot/core/filters/ElementCriterion.h>
#include <hoot/core/filters/ElementCriterionConsumer.h>
#include <hoot/core/visitors/SingleStatistic.h>

/**
 * A visitor for finding the worst circular error among elements. Worst = greatest.
 */
namespace hoot
{

class WorstCircularErrorVisitor :
    public ElementConstOsmMapVisitor,
    public SingleStatistic
{
public:
  static string className() { return "hoot::WorstCircularErrorVisitor"; }

  WorstCircularErrorVisitor(): _worst(-1) {}

  Meters getWorstCircularError() { return _worst; }

  virtual double getStat() const { return _worst; }

  virtual void setOsmMap(const OsmMap* map) { _map = map; }

  virtual void visit(const shared_ptr<const Element>& e)
  {
    _worst = max(_worst, e->getCircularError());
  }

  // Convenient way to get worst circular error
  static Meters getWorstCircularError(const OsmMapPtr& map)
  {
    WorstCircularErrorVisitor v;
    map->visitNodesRo(v);
    map->visitWaysRo(v);
    return v.getWorstCircularError();
  }

  // Handle const pointers to const
  static Meters getWorstCircularError(const ConstOsmMapPtr& map)
  {
    WorstCircularErrorVisitor v;
    map->visitNodesRo(v);
    map->visitWaysRo(v);
    return v.getWorstCircularError();
  }

private:

  const OsmMap* _map;
  Meters _worst;
};

} // end namespace hoot

#endif // WORSTCIRCULARERRORVISITOR_H
