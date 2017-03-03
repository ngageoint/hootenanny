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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "KeepReviewsVisitor.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/ops/RemoveElementOp.h>
#include <hoot/core/filters/NeedsReviewCriterion.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, KeepReviewsVisitor)

void KeepReviewsVisitor::visit(const ConstElementPtr& e)
{
  ConstOsmMapPtr map(_map->shared_from_this());
  NeedsReviewCriterion crit(map);
  if (!crit.isSatisfied(e))
  {
    /// This could do bad things if the element is in use.
    RemoveElementOp::removeElementNoCheck(_map->shared_from_this(), e->getElementId());
  }
}

}
