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
 * @copyright Copyright (C) 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#include "DuplicateElementMarker.h"

// Hoot
#include <hoot/core/criterion/WayNodeCriterion.h>
#include <hoot/core/elements/ElementDeduplicator.h>
#include <hoot/core/elements/WayUtils.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/UuidHelper.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, DuplicateElementMarker)

DuplicateElementMarker::DuplicateElementMarker()
  : _coordinateComparisonSensitivity(ConfigOptions().getNodeComparisonCoordinateSensitivity())
{
}

void DuplicateElementMarker::apply(OsmMapPtr& map)
{
  _numAffected = 0;

  QMap<QString, ElementId> hashes;
  QSet<std::pair<ElementId, ElementId>> duplicates;
  LOG_VARD(_coordinateComparisonSensitivity);
  ElementDeduplicator::calculateDuplicateElements(map, hashes, duplicates, _coordinateComparisonSensitivity);
  // don't care about the hashes
  hashes.clear();

  WayNodeCriterion wayNodeCrit(map);
  for (const auto& dupes : qAsConst(duplicates))
  {
    ElementPtr dupe1 = map->getElement(dupes.first);
    ElementPtr dupe2 = map->getElement(dupes.second);
    if (dupe1 && dupe2 && dupe1->getElementId() != dupe2->getElementId())
    {
      const QString uuid = UuidHelper::createUuid().toString();
      dupe1->setTag(MetadataTags::HootDuplicate(), _getUuidVal(uuid, dupe1));
      dupe2->setTag(MetadataTags::HootDuplicate(), _getUuidVal(uuid, dupe2));
      _numAffected++;

      if (wayNodeCrit.isSatisfied(dupe1))
      {
        const std::set<QString> containingWayTypesTemp = WayUtils::getContainingWaysMostSpecificTypes(dupe1->getId(), map);
        _containingWayTypes.insert(containingWayTypesTemp.begin(), containingWayTypesTemp.end());
      }
      if (wayNodeCrit.isSatisfied(dupe2))
      {
        const std::set<QString> containingWayTypesTemp = WayUtils::getContainingWaysMostSpecificTypes(dupe2->getId(), map);
        _containingWayTypes.insert(containingWayTypesTemp.begin(), containingWayTypesTemp.end());
      }
    }
  }

  _numProcessed = map->size();
}

QString DuplicateElementMarker::_getUuidVal(const QString& newUuid, const ConstElementPtr& element) const
{
  QString uuidVal;
  QString existingUuid = element->getTag(MetadataTags::HootDuplicate()).trimmed();
  if (!existingUuid.isEmpty())
    uuidVal = existingUuid + ";";
  uuidVal += newUuid;
  return uuidVal;
}

}
