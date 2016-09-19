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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "RemoveElementsVisitor.h"

#include <hoot/core/Factory.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/conflate/NodeToWayMap.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/ops/RemoveElementOp.h>
#include <hoot/core/util/ConfigOptions.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, RemoveElementsVisitor)

RemoveElementsVisitor::RemoveElementsVisitor()
{
  setConfiguration(conf());
}

RemoveElementsVisitor::RemoveElementsVisitor(const shared_ptr<ElementCriterion>& filter) :
  _filter(filter),
  _recursive(false)
{
}

void RemoveElementsVisitor::setConfiguration(const Settings& conf)
{
  ConfigOptions configOptions(conf);
  QString filterName = configOptions.getRemoveElementsVisitorFilter();
  if (filterName.isEmpty() == false)
  {
    ElementCriterion* ef = Factory::getInstance().constructObject<ElementCriterion>(filterName);
    _filter.reset(ef);
  }
  _recursive = configOptions.getRemoveElementsVisitorRecursive();
}

void RemoveElementsVisitor::visit(const ConstElementPtr& e)
{
  assert(_filter);
  ElementType type = e->getElementType();
  long id = e->getId();
  const shared_ptr<Element>& ee = _map->getElement(type, id);

  if (_filter->isSatisfied(ee))
  {
    if (_recursive)
    {
      RecursiveElementRemover(ee->getElementId()).apply(_map->shared_from_this());
    }
    else
    {
      RemoveElementOp::removeElement(_map->shared_from_this(), ElementId(type, id));
    }
  }
}

void RemoveElementsVisitor::removeWays(shared_ptr<OsmMap> pMap,
                       const shared_ptr<ElementCriterion>& pCrit)
{
  RemoveElementsVisitor v(pCrit);
  pMap->visitWaysRw(v);
}

}

