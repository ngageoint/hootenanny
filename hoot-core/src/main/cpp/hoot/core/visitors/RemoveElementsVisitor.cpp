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
#include "RemoveElementsVisitor.h"

#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/elements/NodeToWayMap.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/ops/RemoveElementOp.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/criterion/NotCriterion.h>
#include <hoot/core/util/Log.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, RemoveElementsVisitor)

RemoveElementsVisitor::RemoveElementsVisitor(bool negateCriteria) :
_recursive(false),
_count(0),
_negateCriteria(negateCriteria),
_chainCriteria(false)
{
}

void RemoveElementsVisitor::setConfiguration(const Settings& conf)
{
  ConfigOptions configOptions(conf);

  _negateCriteria = configOptions.getElementCriterionNegate();


  const QStringList critNames = configOptions.getRemoveElementsVisitorElementCriteria();
  LOG_VART(critNames);
  if (critNames.size() > 0)
  {
    _criteria.clear();
    for (int i = 0; i < critNames.size(); i++)
    {
      const QString critName = critNames.at(i);
      if (!critName.trimmed().isEmpty())
      {
        LOG_VARD(critName);
        ElementCriterionPtr crit =
          boost::shared_ptr<ElementCriterion>(
            Factory::getInstance().constructObject<ElementCriterion>(critName.trimmed()));
        addCriterion(crit);
        Configurable* c = dynamic_cast<Configurable*>(crit.get());
        if (c != 0)
        {
          c->setConfiguration(conf);
        }
      }
    }
  }

  _recursive = configOptions.getRemoveElementsVisitorRecursive();
  _chainCriteria = configOptions.getRemoveElementsVisitorChainElementCriteria();
  LOG_VARD(_chainCriteria);
}

void RemoveElementsVisitor::setOsmMap(OsmMap* map)
{
  _map = map;

  for (std::vector<ElementCriterionPtr>::const_iterator it = _criteria.begin();
       it != _criteria.end(); ++it)
  {
    ElementCriterionPtr crit = *it;
    OsmMapConsumer* consumer = dynamic_cast<OsmMapConsumer*>(crit.get());
    if (consumer != 0)
      consumer->setOsmMap(map);
  }
}

void RemoveElementsVisitor::addCriterion(const ElementCriterionPtr& crit)
{
  LOG_VART(_negateCriteria);
  LOG_VART(crit.get());
  if (_negateCriteria)
  {
    _criteria.push_back(ElementCriterionPtr(new NotCriterion(crit)));
  }
  else
  {
    _criteria.push_back(ElementCriterionPtr(crit));
  }
}

bool RemoveElementsVisitor::_criteriaSatisfied(const ConstElementPtr& e) const
{
  bool criteriaSatisfied;
  if (!_chainCriteria)
  {
    criteriaSatisfied = false;
    for (std::vector<ElementCriterionPtr>::const_iterator it = _criteria.begin();
         it != _criteria.end(); ++it)
    {
      ElementCriterionPtr crit = *it;
      if (crit->isSatisfied(e))
      {
        criteriaSatisfied = true;
        break;
      }
    }
  }
  else
  {
    criteriaSatisfied = true;
    for (std::vector<ElementCriterionPtr>::const_iterator it = _criteria.begin();
         it != _criteria.end(); ++it)
    {
      ElementCriterionPtr crit = *it;
      if (!crit->isSatisfied(e))
      {
        criteriaSatisfied = false;
        break;
      }
    }
  }
  return criteriaSatisfied;
}

void RemoveElementsVisitor::visit(const ConstElementPtr& e)
{
  LOG_VART(_criteria.size());
  if (_criteria.size() == 0)
  {
    throw IllegalArgumentException("No criteria specified for RemoveElementsVisitor.");
  }

  const ElementType type = e->getElementType();
  const long id = e->getId();
  const boost::shared_ptr<Element>& ee = _map->getElement(type, id);

  if (_criteriaSatisfied(ee))
  {
    LOG_DEBUG("Passed filter: " << e);
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
  RemoveElementsVisitor v;
  v.addCriterion(pCrit);
  pMap->visitWaysRw(v);
}

}

