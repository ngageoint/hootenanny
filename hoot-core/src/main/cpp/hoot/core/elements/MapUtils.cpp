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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "MapUtils.h"

// Hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/ops/CopyMapSubsetOp.h>
#include <hoot/core/visitors/ElementCountVisitor.h>
#include <hoot/core/visitors/FilteredVisitor.h>
#include <hoot/core/criterion/PointCriterion.h>

namespace hoot
{

OsmMapPtr MapUtils::getMapSubset(const ConstOsmMapPtr& map, const ElementCriterionPtr& filter)
{
  CopyMapSubsetOp wayCopier(map, filter);
  OsmMapPtr output(new OsmMap());
  wayCopier.apply(output);
  return output;
}

bool MapUtils::mapIsPointsOnly(const OsmMapPtr& map)
{
  std::shared_ptr<PointCriterion> pointCrit(new PointCriterion());
  pointCrit->setOsmMap(map.get());
  return
    (int)FilteredVisitor::getStat(
      pointCrit, ElementVisitorPtr(new ElementCountVisitor()), map) ==
    (int)map->getElementCount();
}

}
