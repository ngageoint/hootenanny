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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#include "WorstCircularErrorVisitor.h"

// hoot
#include <hoot/core/util/Factory.h>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, WorstCircularErrorVisitor)

void WorstCircularErrorVisitor::visit(const std::shared_ptr<const Element>& e)
{
  _worst = max(_worst, e->getCircularError());
}

Meters WorstCircularErrorVisitor::getWorstCircularError(const OsmMapPtr& map)
{
  WorstCircularErrorVisitor v;
  map->visitNodesRo(v);
  map->visitWaysRo(v);
  return v.getWorstCircularError();
}

Meters WorstCircularErrorVisitor::getWorstCircularError(const ConstOsmMapPtr& map)
{
  WorstCircularErrorVisitor v;
  map->visitNodesRo(v);
  map->visitWaysRo(v);
  return v.getWorstCircularError();
}

Meters WorstCircularErrorVisitor::getWorstCircularError(const std::vector<ElementPtr>& elements)
{
  WorstCircularErrorVisitor v;
  for (const auto& element : elements)
    v.visit(element);
  return v.getWorstCircularError();
}

}
