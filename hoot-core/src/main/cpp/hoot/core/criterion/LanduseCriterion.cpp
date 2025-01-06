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
 * @copyright Copyright (C) 2019-2023 Maxar (http://www.maxar.com/)
 */

#include "LanduseCriterion.h"

// hoot
#include <hoot/core/criterion/PolygonWayNodeCriterion.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, LanduseCriterion)

LanduseCriterion::LanduseCriterion(ConstOsmMapPtr map)
  : ConstOsmMapConsumerBase(map)
{
    // Set this to allow any on poly child member to satisfy the crit.
    _relationCrit.setAllowMixedChildren(true);
    _relationCrit.setOsmMap(map.get());
}

void LanduseCriterion::setOsmMap(const OsmMap* map)
{
    ConstOsmMapConsumerBase::setOsmMap(map);
    _relationCrit.setOsmMap(map);
}

bool LanduseCriterion::isSatisfied(const ConstElementPtr& e) const
{
    LOG_VART(e->getElementId());

    // element has landuse tag
    const Tags& tags = e->getTags();
    auto it = tags.find("landuse");
    // Specifically looking for BUAs
    std::vector<QString> landuseTypes = {"residential", "commercial", "industrial"};
    const bool containsLanduseTag = it != tags.end() && (std::find(landuseTypes.begin(), landuseTypes.end(), it.value()) != landuseTypes.end());
    bool result = false;

    switch(e->getElementType().getEnum())
    {
        default:
        case ElementType::Node:
            return false;
        case ElementType::Way:
        {
            ConstWayPtr way = std::dynamic_pointer_cast<const Way>(e);
            LOG_VART(way->isValidPolygon());
            LOG_VART(way->isClosedArea());
            if (way->isValidPolygon() && way->isClosedArea() && containsLanduseTag)
            {
                LOG_TRACE("Way is valid closed landuse area; crit satisfied.");
                result = true;
                LOG_TRACE(e->getElementId() << " result: " << result);
                return true;
            }
            break;
        }
    }
    LOG_TRACE(e->getElementId() << " result: " << result);
    return false;
}

QStringList LanduseCriterion::getChildCriteria() const
{
    return QStringList(PolygonWayNodeCriterion::className());
}

}