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
 * @copyright Copyright (C) 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#include "PolyToWayGeoModifierAction.h"

// Hoot
#include <hoot/core/util/Factory.h>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(GeometryModifierAction, PolyToWayGeoModifierAction)

const QString PolyToWayGeoModifierAction::NOOP_PARAM = "nothing";

bool PolyToWayGeoModifierAction::processElement(const ElementPtr& pElement, OsmMap* pMap)
{
  // NOTE: The need for this code been met by a combination of AreaCriterion, TagCriterion and a SetTagVisitor
  // The code will be removed if, after more testing, the Criterion/Visitor approach is successful
  // Currently, this is faster than using the Criterion.

  // only process closed area ways
  if (pElement->getElementType() != ElementType::Way) return false;
  const WayPtr& pWay = std::dynamic_pointer_cast<Way>(pElement);
  if (!pWay->isClosedArea()) return false;

  // Keep the compiler happy and avoid Unused Variable warnings.
  LOG_VART(pMap->size());

  // Trying this as an initial method.
  // If this hits issues with multipolygons then we may need to do more geometry tweaking.
  pElement->getTags()["area"] = "no";
  LOG_TRACE("Adding area=no tag");

  return true;
}

void PolyToWayGeoModifierAction::parseArguments(const QHash<QString, QString>& arguments)
{
  // Keep the compiler happy
  if (arguments.keys().size() > 0)
  {
    LOG_WARN("Poly to Way modifier does not take arguments");
  }
}

}
