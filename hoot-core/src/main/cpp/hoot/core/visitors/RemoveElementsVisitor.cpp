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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "RemoveElementsVisitor.h"

#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/conflate/NodeToWayMap.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/ops/RemoveElementOp.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/criterion/NotCriterion.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, RemoveElementsVisitor)

RemoveElementsVisitor::RemoveElementsVisitor():
_count(0),
_negateCriterion(false)
{
  setConfiguration(conf());
}

RemoveElementsVisitor::RemoveElementsVisitor(const boost::shared_ptr<ElementCriterion>& criterion,
                                             bool negateCriterion) :
_criterion(criterion),
_recursive(false),
_count(0),
_negateCriterion(negateCriterion)
{
  if (_negateCriterion)
  {
    _criterion.reset(new NotCriterion(criterion));
  }
}

void RemoveElementsVisitor::setConfiguration(const Settings& conf)
{
  ConfigOptions configOptions(conf);
  _negateCriterion = configOptions.getElementCriterionNegate();
  LOG_VART(_negateCriterion);
  const QString critName = configOptions.getRemoveElementsVisitorElementCriterion();
  LOG_VART(critName);
  if (critName.isEmpty() == false)
  {
    LOG_VART(critName);
    addCriterion(
      boost::shared_ptr<ElementCriterion>(
        Factory::getInstance().constructObject<ElementCriterion>(critName.trimmed())));
  }
  _recursive = configOptions.getRemoveElementsVisitorRecursive();
  LOG_VART(_recursive);
}

void RemoveElementsVisitor::setOsmMap(OsmMap* map)
{
  _map = map;
  OsmMapConsumer* consumer = dynamic_cast<OsmMapConsumer*>(_criterion.get());
  if (consumer != 0)
    consumer->setOsmMap(map);
}


void RemoveElementsVisitor::addCriterion(const ElementCriterionPtr& e)
{
  if (!_negateCriterion)
  {
    _criterion = e;
  }
  else
  {
    _criterion.reset(new NotCriterion(e));
  }
}

void RemoveElementsVisitor::visit(const ConstElementPtr& e)
{
  assert(_criterion);
  ElementType type = e->getElementType();
  long id = e->getId();
  LOG_VART(id);
  const boost::shared_ptr<Element>& ee = _map->getElement(type, id);

  if (_criterion->isSatisfied(ee))
  {
    LOG_TRACE("RemoveElementsVisitor criterion satisfied");
    LOG_VART(_recursive);
    _count++;
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

void RemoveElementsVisitor::removeWays(boost::shared_ptr<OsmMap> pMap,
                       const boost::shared_ptr<ElementCriterion>& pCrit)
{
  RemoveElementsVisitor v(pCrit);
  pMap->visitWaysRw(v);
}

}

