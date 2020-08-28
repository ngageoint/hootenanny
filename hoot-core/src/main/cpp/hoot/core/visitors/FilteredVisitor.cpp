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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "FilteredVisitor.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/ConstOsmMapConsumer.h>
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/info/SingleStatistic.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, FilteredVisitor)

FilteredVisitor::FilteredVisitor(const ElementCriterion& criterion, ElementVisitor& visitor) :
  _criterion(&criterion),
  _visitor(&visitor)
{
}

FilteredVisitor::FilteredVisitor(const ElementCriterion& criterion, ElementVisitorPtr visitor) :
  _criterion(&criterion),
  _visitor(visitor.get())
{
}

FilteredVisitor::FilteredVisitor(ElementCriterionPtr criterion, ElementVisitorPtr visitor) :
  _criterion(criterion.get()),
  _visitor(visitor.get())
{
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
  LOG_VART(e->getElementId());
//  if (_criterion != 0)
//  {
//    LOG_VART(_criterion->toString());
//  }
  if (_criterion->isSatisfied(e))
  {
    LOG_TRACE("crit satisfied for: " << e->getElementId());
    // This is bad. Making this change was the result of a cascading set of const correctness
    // changes necessary in order to be able to call ElementVisitor from js files and not just
    // ConstElementVisitor (#2831). We may need some re-design.
    _visitor->visit(std::const_pointer_cast<Element>(e));
  }
  else
  {
    LOG_TRACE("crit not satisfied for: " << e->getElementId());
  }
}

double FilteredVisitor::getStat(ElementCriterionPtr criterion, ElementVisitorPtr visitor,
                                const ConstOsmMapPtr& map)
{
  FilteredVisitor filteredVisitor(criterion, visitor);
  SingleStatistic* stat = dynamic_cast<SingleStatistic*>(&filteredVisitor.getChildVisitor());
  if (stat == 0)
  {
    throw HootException("Visitor does not implement SingleStatistic.");
  }
  FilteredVisitor& filteredVis = const_cast<FilteredVisitor&>(filteredVisitor);
  map->visitRo(filteredVis);
  return stat->getStat();
}

double FilteredVisitor::getStat(ElementCriterion* criterion, ElementVisitor* visitor,
                                const ConstOsmMapPtr& map, const ElementPtr& element)
{
  FilteredVisitor filteredVisitor(criterion, visitor);
  SingleStatistic* stat = dynamic_cast<SingleStatistic*>(&filteredVisitor.getChildVisitor());
  if (stat == 0)
  {
    throw HootException("Visitor does not implement SingleStatistic.");
  }
  element->visitRo(*map, filteredVisitor);
  return stat->getStat();
}

}
