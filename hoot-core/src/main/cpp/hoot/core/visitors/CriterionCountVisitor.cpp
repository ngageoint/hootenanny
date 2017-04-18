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
 * @copyright Copyright (C) 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "CriterionCountVisitor.h"
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/OsmMap.h>

namespace hoot
{

CriterionCountVisitor::CriterionCountVisitor():
  _map(NULL),
  _count(0),
  _visited(0),
  _pCrit()
{
  setConfiguration(conf());
}

CriterionCountVisitor::CriterionCountVisitor(const shared_ptr<ElementCriterion>& pCrit):
  _map(NULL),
  _count(0),
  _visited(0),
  _pCrit(pCrit)
{
  setConfiguration(conf());
}

void CriterionCountVisitor::setConfiguration(const Settings& conf)
{
  ConfigOptions configOptions(conf);
  (void) configOptions; // Do nothing
}

void CriterionCountVisitor::visit(const ConstElementPtr& e)
{
  assert(_pCrit);
  ElementType type = e->getElementType();
  long id = e->getId();
  const shared_ptr<const Element>& ee = _map->getElement(type, id);

  if (_pCrit->isSatisfied(ee))
  {
    ++_count;
  }
  ++_visited;
}

}
