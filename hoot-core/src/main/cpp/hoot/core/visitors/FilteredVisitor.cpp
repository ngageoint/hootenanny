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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "FilteredVisitor.h"

// hoot
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/elements/ConstOsmMapConsumer.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/info/SingleStatistic.h>
#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, FilteredVisitor)

FilteredVisitor::FilteredVisitor()
  : _criterion(nullptr),
    _visitor(nullptr)
{
}

FilteredVisitor::FilteredVisitor(const ElementCriterion& criterion, ElementVisitor& visitor)
  : _criterion(&criterion),
    _visitor(&visitor),
    _map(nullptr)
{
}

FilteredVisitor::FilteredVisitor(ElementCriterionPtr criterion, ElementVisitorPtr visitor)
  : _criterion(criterion.get()),
    _visitor(visitor.get()),
    _map(nullptr)
{
}

void FilteredVisitor::addCriterion(const ElementCriterionPtr& e)
{
  if (_criterion)
  {
    throw IllegalArgumentException("FilteredVisitor only takes one criterion.");
  }
  _criterion = e.get();
}

void FilteredVisitor::addVisitor(const ElementVisitorPtr& v)
{
  if (_visitor)
    throw IllegalArgumentException("FilteredVisitor only takes one visitor.");
  _visitor = v.get();
}

void FilteredVisitor::setOsmMap(OsmMap* map)
{
  ConstOsmMapConsumer* c = dynamic_cast<ConstOsmMapConsumer*>(_visitor);
  if (c != nullptr)
    c->setOsmMap(map);
  _map = map;
}

void FilteredVisitor::setOsmMap(const OsmMap* map)
{
  ConstOsmMapConsumer* c = dynamic_cast<ConstOsmMapConsumer*>(_visitor);
  if (c != nullptr)
    c->setOsmMap(map);
  _map = map;
}

void FilteredVisitor::visit(const ConstElementPtr& e)
{
  LOG_VART(e->getElementId());
  if (_criterion->isSatisfied(e))
  {
    LOG_TRACE(
      "crit: " << _criterion->toString() << " satisfied for: " << e->getElementId() <<
      ". Calling visitor: " << _visitor->getName());
    // This is bad. Making this change was the result of a cascading set of const correctness
    // changes necessary in order to be able to call ElementVisitor from js files and not just
    // ConstElementVisitor (#2831). We may need some re-design.
    _visitor->visit(std::const_pointer_cast<Element>(e));
  }
  else
  {
    LOG_TRACE("crit: " << _criterion->toString() << " not satisfied for: " << e->getElementId());
  }
}

double FilteredVisitor::getStat(ElementCriterionPtr criterion, ElementVisitorPtr visitor,
                                const ConstOsmMapPtr& map)
{
  FilteredVisitor filteredVisitor(criterion, visitor);
  const SingleStatistic* stat = dynamic_cast<SingleStatistic*>(&filteredVisitor.getChildVisitor());
  if (stat == nullptr)
    throw HootException("Visitor does not implement SingleStatistic.");
  FilteredVisitor& filteredVis = const_cast<FilteredVisitor&>(filteredVisitor);
  map->visitRo(filteredVis);
  return stat->getStat();
}

double FilteredVisitor::getStat(ElementCriterionPtr criterion, ElementVisitorPtr visitor,
                                const ConstOsmMapPtr& map, const ElementPtr& element)
{
  return getStat(*criterion, *visitor, map, element);
}

double FilteredVisitor::getStat(const ElementCriterion& criterion, ElementVisitor& visitor,
                                const ConstOsmMapPtr& map, const ElementPtr& element)
{
  FilteredVisitor filteredVisitor(criterion, visitor);
  const SingleStatistic* stat = dynamic_cast<SingleStatistic*>(&filteredVisitor.getChildVisitor());
  if (stat == nullptr)
    throw HootException("Visitor does not implement SingleStatistic.");
  element->visitRo(*map, filteredVisitor);
  return stat->getStat();
}

}
