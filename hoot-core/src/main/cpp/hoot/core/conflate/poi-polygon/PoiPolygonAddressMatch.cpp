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
  QStringList addresses;
  Tags buildingTags = building->getTags();

  QString buildingHouseNum = buildingTags.get("addr:housenumber").trimmed();
  QString buildingStreet =
    Translator::getInstance().toEnglish(buildingTags.get("addr:street")).trimmed().toLower();
  QString buildingAddrComb;
  if (!buildingHouseNum.isEmpty() && !buildingStreet.isEmpty())
  {
    buildingAddrComb = buildingHouseNum + " " + buildingStreet;
    addresses.append(buildingAddrComb);
  }
  QString buildingAddrTag =
    Translator::getInstance().toEnglish(buildingTags.get("address")).trimmed().toLower();
  if (!buildingAddrTag.isEmpty())
  {
    addresses.append(buildingAddrTag);
  }

  if (addresses.size() == 0)
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
        if (!buildingAddrTag.isEmpty())
        {
          addresses.append(buildingAddrTag);
        }
        if (!buildingHouseNum.isEmpty() && !buildingStreet.isEmpty())
        {
          buildingAddrComb = buildingHouseNum + " " + buildingStreet;
          addresses.append(buildingAddrComb);
        }
      }
    }
    //haven't seen addresses yet in building relation node members
    /*else if (e2->getElementType() == ElementType::Relation)
      {
      }*/
  }
  if (addresses.size() == 0)
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
  for (int i = 0; i < addresses.size(); i++)
  {
    const QString buildingAddress = addresses.at(i);
    if (addrComp.compare(buildingAddress, poiAddrTag) == 1.0 ||
        addrComp.compare(buildingAddress, poiAddrComb) == 1.0)
    {
      return true;
    }
  }
  return false;
}

}
