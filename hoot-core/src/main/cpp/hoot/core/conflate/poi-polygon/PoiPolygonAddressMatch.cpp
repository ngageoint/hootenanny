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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "PoiPolygonAddressMatch.h"

// hoot
#include <hoot/core/algorithms/Translator.h>
#include <hoot/core/algorithms/ExactStringDistance.h>

namespace hoot
{

PoiPolygonAddressMatch::PoiPolygonAddressMatch(const ConstOsmMapPtr& map,
                                               const QString testUuid = "") :
_map(map),
_testUuid(testUuid)
{
}

bool PoiPolygonAddressMatch::calculateMatch(ConstElementPtr building, ConstElementPtr poi)
{
  Tags buildingTags = building->getTags();
  QString buildingHouseNum = buildingTags.get("addr:housenumber").trimmed();
  QString buildingStreet =
    Translator::getInstance().toEnglish(buildingTags.get("addr:street")).trimmed().toLower();
  QString buildingAddrComb;
  if (!buildingHouseNum.isEmpty() && !buildingStreet.isEmpty())
  {
    buildingAddrComb = buildingHouseNum + " " + buildingStreet;
  }
  QString buildingAddrTag =
    Translator::getInstance().toEnglish(buildingTags.get("address")).trimmed().toLower();
  if (buildingAddrComb.isEmpty() && buildingAddrTag.isEmpty())
  {
    //try to find the address from a building way node instead
    if (building->getElementType() == ElementType::Way)
    {
      ConstWayPtr wayBuilding = dynamic_pointer_cast<const Way>(building);
      const vector<long> wayNodeIds = wayBuilding->getNodeIds();
      for (size_t i = 0; i < wayNodeIds.size(); i++)
      {
        ConstElementPtr buildingWayNode = _map->getElement(ElementType::Node, wayNodeIds.at(i));
        buildingTags = buildingWayNode->getTags();
        buildingHouseNum = buildingTags.get("addr:housenumber").trimmed();
        buildingStreet =
          Translator::getInstance().toEnglish(buildingTags.get("addr:street")).trimmed().toLower();
        buildingAddrTag =
          Translator::getInstance().toEnglish(buildingTags.get("address")).trimmed().toLower();
        if (!buildingHouseNum.isEmpty() && !buildingStreet.isEmpty())
        {
          buildingAddrComb = buildingHouseNum + " " + buildingStreet;
        }
        if (!buildingAddrComb.isEmpty() || !buildingAddrTag.isEmpty())
        {
          break;
        }
      }
    }
    //haven't seen addresses yet in building relation node members
    /*else if (e2->getElementType() == ElementType::Relation)
      {
      }*/
  }
  if (buildingAddrComb.isEmpty() && buildingAddrTag.isEmpty())
  {
    return false;
  }

  const Tags poiTags = poi->getTags();
  const QString poiHouseNum = poiTags.get("addr:housenumber").trimmed();
  const QString poiStreet =
    Translator::getInstance().toEnglish(poiTags.get("addr:street")).trimmed().toLower();
  QString poiAddrComb;
  if (!poiHouseNum.isEmpty() && !poiStreet.isEmpty())
  {
    poiAddrComb = poiHouseNum + " " + poiStreet;
  }
  const QString poiAddrTag =
    Translator::getInstance().toEnglish(poiTags.get("address")).trimmed().toLower();
  if (poiAddrComb.isEmpty() && poiAddrTag.isEmpty())
  {
    return false;
  }

  if (Log::getInstance().getLevel() == Log::Debug &&
      (buildingTags.get("uuid") == _testUuid || poiTags.get("uuid") == _testUuid))
  {
    LOG_VARD(buildingAddrComb);
    LOG_VARD(poiAddrComb);
    LOG_VARD(buildingAddrTag);
    LOG_VARD(poiAddrTag);
  }

  ExactStringDistance addrComp;
  return
    (!buildingAddrTag.isEmpty() && !poiAddrTag.isEmpty() &&
       addrComp.compare(buildingAddrTag, poiAddrTag) == 1.0) ||
    (!buildingAddrComb.isEmpty() && !poiAddrTag.isEmpty() &&
       addrComp.compare(buildingAddrComb, poiAddrTag) == 1.0) ||
    (!poiAddrComb.isEmpty() && !buildingAddrTag.isEmpty() &&
       addrComp.compare(poiAddrComb, buildingAddrTag) == 1.0) ||
    (!buildingAddrComb.isEmpty() && !poiAddrComb.isEmpty() &&
       addrComp.compare(buildingAddrComb, poiAddrComb) == 1.0);
}

}
