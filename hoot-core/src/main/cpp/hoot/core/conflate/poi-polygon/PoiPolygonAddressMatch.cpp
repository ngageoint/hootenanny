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

const QChar PoiPolygonAddressMatch::ESZETT(0x00DF);
const QString PoiPolygonAddressMatch::ESZETT_REPLACE = "ss";
const QString PoiPolygonAddressMatch::HOUSE_NUMBER_TAG_NAME = "addr:housenumber";
const QString PoiPolygonAddressMatch::STREET_TAG_NAME = "addr:street";
const QString PoiPolygonAddressMatch::FULL_ADDRESS_TAG_NAME = "address";
const QString PoiPolygonAddressMatch::FULL_ADDRESS_TAG_NAME_2 = "addr:full";

PoiPolygonAddressMatch::PoiPolygonAddressMatch(const ConstOsmMapPtr& map,
                                               const QString testUuid = "") :
_map(map),
_testUuid(testUuid)
{
}

void PoiPolygonAddressMatch::_parseAddressesAsRange(const QString houseNum, const QString street,
                                                    QStringList& addresses)
{
  //address ranges; e.g. 1-3 elm street is an address range that includes the addresses:
  //"1 elm street", "2 elm street", and "3 elm street".  I've only seen this on the houseNum
  //tag so far, but conceivably you could find it in the full address tags as well...won't add
  //logic for that, though, unless its encountered.  I could also see someone separating '-'
  //with a space (which isn't hard to handle), but also won't worry about it until its seen
  //in the wild.

  QStringList houseNumParts = houseNum.split("-");
  //LOG_VARD(houseNumParts);
  if (houseNumParts.size() == 2)
  {
    bool startHouseNumParsedOk = false;
    const int startHouseNum = houseNumParts[0].toInt(&startHouseNumParsedOk);
    //LOG_VARD(startHouseNum);
    if (startHouseNumParsedOk)
    {
      bool endHouseNumParsedOk = false;
      const int endHouseNum = houseNumParts[1].toInt(&endHouseNumParsedOk);
      //LOG_VARD(endHouseNum);
      if (endHouseNumParsedOk && startHouseNum < endHouseNum)
      {
        QString combinedAddress;
        for (int i = startHouseNum; i < endHouseNum + 1; i++)
        {
          combinedAddress = QString::number(i) + " " + street;
          //LOG_VARD(combinedAddress);
          addresses.append(combinedAddress);
        }
      }
    }
  }
}

void PoiPolygonAddressMatch::_parseAddressesInAltFormat(const Tags& tags, QStringList& addresses)
{
  QString addressTagValAltFormatRaw =
    Translator::getInstance().toEnglish(tags.get(FULL_ADDRESS_TAG_NAME_2)).trimmed().toLower();
  if (!addressTagValAltFormatRaw.isEmpty())
  {
    addressTagValAltFormatRaw = addressTagValAltFormatRaw.replace(ESZETT, ESZETT_REPLACE);
    //LOG_VARD(addressTagValAltFormatRaw);
    const QStringList addressParts = addressTagValAltFormatRaw.split(QRegExp("\\s"));
    //LOG_VARD(addressParts);
    if (addressParts.length() >= 2)
    {
      QString addressTagValAltFormat = "";
      bool ok = false;
      int ctr = 1;
      while (ctr < addressParts.length() && !ok)
      {
        addressParts[ctr].toInt(&ok);
        ctr++;
      }
      //LOG_VARD(ctr);
      //LOG_VARD(ok);
      if (ok && ctr > 1)
      {
        const QString houseNum = addressParts[ctr - 1];
        //LOG_VARD(houseNum);
        addressTagValAltFormat += houseNum;
        //LOG_VARD(addressTagValAltFormat);
        for (int i = 0; i < (ctr - 1); i++)
        {
          addressTagValAltFormat += " ";
          addressTagValAltFormat += addressParts[i];
          //LOG_VARD(addressTagValAltFormat);
        }
        addressTagValAltFormat = addressTagValAltFormat.trimmed();
        //LOG_VARD(addressTagValAltFormat);
        addresses.append(addressTagValAltFormat);
      }
    }
  }
}

void PoiPolygonAddressMatch::_collectAddressesFromElement(ConstElementPtr element,
                                                          QStringList& addresses)
{
  const Tags tags = element->getTags();

  //address parts in separate tags (most common situation)
  QString houseNum = tags.get(HOUSE_NUMBER_TAG_NAME).trimmed();
  QString street =
    Translator::getInstance().toEnglish(tags.get(STREET_TAG_NAME)).trimmed().toLower();
  QString combinedAddress;
  if (!houseNum.isEmpty() && !street.isEmpty())
  {
    //TODO: hack - I thought this would have been eliminated by using the translated name comparison
    //logic...seems like it wasn't. - see others
    street = street.replace(ESZETT, ESZETT_REPLACE);
    if (!houseNum.contains("-"))
    {
      combinedAddress = houseNum + " " + street;
      addresses.append(combinedAddress);
    }
    else
    {
      _parseAddressesAsRange(houseNum, street, addresses);
    }
  }

  //full address in one tag
  QString addressTagVal =
    Translator::getInstance().toEnglish(tags.get(FULL_ADDRESS_TAG_NAME)).trimmed().toLower();
  if (!addressTagVal.isEmpty())
  {
    addressTagVal = addressTagVal.replace(ESZETT, ESZETT_REPLACE);
    addresses.append(addressTagVal);
  }

  //street name and house num reversed: ZENTRALLÄNDSTRASSE 40 81379 MÜNCHEN
  //parse through the tokens until you come to a number; assume that is the house number and
  //everything before it is the street name
  _parseAddressesInAltFormat(tags, addresses);
}

void PoiPolygonAddressMatch::_collectAddressesFromWay(ConstWayPtr way, QStringList& addresses)
{
  const vector<long> wayNodeIds = way->getNodeIds();
  for (size_t i = 0; i < wayNodeIds.size(); i++)
  {
    _collectAddressesFromElement(_map->getElement(ElementType::Node, wayNodeIds.at(i)), addresses);
  }
}

void PoiPolygonAddressMatch::_collectAddressesFromRelation(ConstRelationPtr relation,
                                                           QStringList& addresses)
{
  const vector<RelationData::Entry> relationMembers = relation->getMembers();
  for (size_t i = 0; i < relationMembers.size(); i++)
  {
    ConstElementPtr member = _map->getElement(relationMembers[i].getElementId());
    if (member->getElementType() == ElementType::Node)
    {
      _collectAddressesFromElement(member, addresses);
    }
    else if (member->getElementType() == ElementType::Way)
    {
      _collectAddressesFromWay(dynamic_pointer_cast<const Way>(member), addresses);
    }
  }
}

bool PoiPolygonAddressMatch::isMatch(ConstElementPtr poly, ConstElementPtr poi)
{
  const bool testFeatureFound =
    poly->getTags().get("uuid") == _testUuid || poi->getTags().get("uuid") == _testUuid;

  QStringList polyAddresses;

  //see if the poly has any address
  _collectAddressesFromElement(poly, polyAddresses);

  if (polyAddresses.size() == 0)
  {
    //if not, try to find the address from a poly way node instead
    if (poly->getElementType() == ElementType::Way)
    {
      _collectAddressesFromWay(dynamic_pointer_cast<const Way>(poly), polyAddresses);
    }
    //if still no luck, try to find the address from a poly way node that is a relation member
    else if (poly->getElementType() == ElementType::Relation)
    {
      _collectAddressesFromRelation(dynamic_pointer_cast<const Relation>(poly), polyAddresses);
    }
  }
  if (polyAddresses.size() == 0)
  {
    //LOG_DEBUG("No poly addresses.");
    return false;
  }

  //see if the poi has an address
  QStringList poiAddresses;
  _collectAddressesFromElement(poi, poiAddresses);
  if (poiAddresses.size() == 0)
  {
    //LOG_DEBUG("No POI addresses.");
    return false;
  }

  ExactStringDistance addrComp;
  for (int i = 0; i < polyAddresses.size(); i++)
  {
    const QString polyAddress = polyAddresses.at(i);
    for (int j = 0; j < poiAddresses.size(); j++)
    {
      const QString poiAddress = poiAddresses.at(j);

      if (testFeatureFound)
      {
        LOG_VARD(polyAddress);
        LOG_VARD(poiAddress);
      }

      //exact match
      if (addrComp.compare(polyAddress, poiAddress) == 1.0)
      {
        LOG_DEBUG("Found address match.");
        return true;
      }
      //subletter fuzziness
      else if (_addressesMatchesOnSubLetter(polyAddress, poiAddress))
      {
        return true;
      }
    }
  }

  return false;
}

bool PoiPolygonAddressMatch::_addressesMatchesOnSubLetter(const QString polyAddress,
                                                         const QString poiAddress)
{
  /*
   * we're also going to allow sub letter differences be
   //matches; ex "34 elm street" matches "34a elm street".  This is b/c the subletters are
   sometimes left out of the addresses, and we'd like to at least end up with a review in that
   situation.
   */
  //TODO: this may be able to be cleaned up with better use of regex's
  const QStringList polyAddressParts = polyAddress.split(QRegExp("\\s"));
  assert(polyAddressParts.length() > 0);
  const QStringList poiAddressParts = poiAddress.split(QRegExp("\\s"));
  assert(poiAddressParts.length() > 0);

  QString polyAddressTemp = polyAddressParts[0];
  const QString polyHouseNumStr = polyAddressTemp.replace(QRegExp("[a-z]+"), "");
  LOG_VARD(polyHouseNumStr);
  bool polyHouseNumOk = false;
  /*const int polyHouseNum = */polyHouseNumStr.toInt(&polyHouseNumOk);

  QString poiAddressTemp = poiAddressParts[0];
  const QString poiHouseNumStr = poiAddressTemp.replace(QRegExp("[a-z]+"), "");
  //LOG_VARD(poiHouseNumStr);
  bool poiHouseNumOk = false;
  /*const int poiHouseNum = */polyHouseNumStr.toInt(&poiHouseNumOk);

  //don't think this check is needed since the addresses have already been parsed...but will
  //leave it here for now
  if (polyHouseNumOk && poiHouseNumOk)
  {
    QString subletterCleanedPolyAddress = polyHouseNumStr;
    for (int k = 1; k < polyAddressParts.length(); k++)
    {
      subletterCleanedPolyAddress += " " + polyAddressParts[k];
    }
    //LOG_VARD(subletterCleanedPolyAddress);

    QString subletterCleanedPoiAddress = poiHouseNumStr;
    for (int k = 1; k < poiAddressParts.length(); k++)
    {
      subletterCleanedPoiAddress += " " + poiAddressParts[k];
    }
    //LOG_VARD(subletterCleanedPoiAddress);

    ExactStringDistance addrComp;
    if (addrComp.compare(subletterCleanedPolyAddress, subletterCleanedPoiAddress) == 1.0)
    {
      LOG_DEBUG("Found address match.");
      return true;
    }
  }

  return false;
}

}
