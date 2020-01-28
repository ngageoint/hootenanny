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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "RemoveEmptyAreasVisitor.h"

// geos
#include <geos/geom/Geometry.h>

// hoot
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/elements/ElementConverter.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/criterion/AreaCriterion.h>
#include <hoot/core/util/StringUtils.h>

using namespace geos::geom;
namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, RemoveEmptyAreasVisitor)

RemoveEmptyAreasVisitor::RemoveEmptyAreasVisitor()
{
}

void RemoveEmptyAreasVisitor::visit(const ConstElementPtr& e)
{
  // no need to visit nodes.
  if (e->getElementType() != ElementType::Node)
  {
    std::shared_ptr<Element> ee = _map->getElement(e->getElementId());
    visit(ee);
  }
}

void RemoveEmptyAreasVisitor::visit(const std::shared_ptr<Element>& e)
{
  if (!_ec.get())
  {
    _ec.reset(new ElementConverter(_map->shared_from_this()));
  }

  LOG_VART(AreaCriterion().isSatisfied(e));
  if (AreaCriterion().isSatisfied(e))
  {
    std::shared_ptr<Geometry> g = _ec->convertToGeometry(e);
    LOG_VART(g.get());
    if (g.get())
    {
      LOG_VART(g->getArea());
    }
    if (g.get() && g->getArea() == 0.0)
    {
      RecursiveElementRemover(e->getElementId()).apply(_map->shared_from_this());
      _numAffected++;
    }
  }

  _numProcessed++;
  if (_numProcessed % 10000 == 0)
  {
    PROGRESS_INFO(
      "\tProcessed " << StringUtils::formatLargeNumber(_numProcessed) <<
      " / " << StringUtils::formatLargeNumber(_map->size()) << " elements for empty area removal.");
  }
}


}
