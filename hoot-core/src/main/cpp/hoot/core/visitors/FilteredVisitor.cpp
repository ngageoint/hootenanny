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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "FilteredVisitor.h"

// hoot
#include <hoot/core/Factory.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/ConstOsmMapConsumer.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, FilteredVisitor)

FilteredVisitor::FilteredVisitor(const ElementCriterion& criterion, ElementVisitor& visitor) :
  _criterion(&criterion),
  _visitor(&visitor)
{
}

FilteredVisitor::FilteredVisitor(const ElementCriterion& criterion, ElementVisitor* visitor) :
  _criterion(&criterion),
  _visitor(visitor)
{
  _visitDelete.reset(visitor);
}

FilteredVisitor::FilteredVisitor(ElementCriterion* criterion, ElementVisitor* visitor) :
  _criterion(criterion),
  _visitor(visitor)
{
  _criterionDelete.reset(criterion);
  _visitDelete.reset(visitor);
}

void FilteredVisitor::addCriterion(const ElementCriterionPtr& e)
{
  if (_criterion)
  {
    throw IllegalArgumentException("FilteredVisitor only takes one criterion.");
  }
  _criterion = e.get();
  _criterionDelete = e;
}

void FilteredVisitor::addVisitor(const ElementVisitorPtr& v)
{
  if (_visitor)
  {
    throw IllegalArgumentException("FilteredVisitor only takes one visitor.");
  }
  _visitor = v.get();
  _visitDelete = v;
}

void FilteredVisitor::setOsmMap(OsmMap* map)
{
  ConstOsmMapConsumer* c = dynamic_cast<ConstOsmMapConsumer*>(_visitor);
  if (c != 0)
  {
    c->setOsmMap(map);
  }
  _map = map;
}

void FilteredVisitor::setOsmMap(const OsmMap* map)
{
  ConstOsmMapConsumer* c = dynamic_cast<ConstOsmMapConsumer*>(_visitor);
  if (c != 0)
  {
    c->setOsmMap(map);
  }
  _map = map;
}
void FilteredVisitor::visit(const ConstElementPtr& e)
{
  if (_criterion->isSatisfied(e))
  {
    _visitor->visit(e);
  }
}

}
