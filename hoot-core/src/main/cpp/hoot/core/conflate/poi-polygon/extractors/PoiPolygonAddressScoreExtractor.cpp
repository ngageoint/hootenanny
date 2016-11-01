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
#include "PoiPolygonAddressScoreExtractor.h"

// hoot
#include <hoot/core/algorithms/Translator.h>
#include <hoot/core/algorithms/ExactStringDistance.h>
#include <hoot/core/algorithms/LevenshteinDistance.h>
#include <hoot/core/algorithms/MeanWordSetDistance.h>
#include <hoot/core/schema/TranslateStringDistance.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(FeatureExtractor, PoiPolygonAddressScoreExtractor)

const QChar PoiPolygonAddressScoreExtractor::ESZETT(0x00DF);
const QString PoiPolygonAddressScoreExtractor::ESZETT_REPLACE = "ss";
const QString PoiPolygonAddressScoreExtractor::HOUSE_NUMBER_TAG_NAME = "addr:housenumber";
const QString PoiPolygonAddressScoreExtractor::STREET_TAG_NAME = "addr:street";
const QString PoiPolygonAddressScoreExtractor::FULL_ADDRESS_TAG_NAME = "address";
const QString PoiPolygonAddressScoreExtractor::FULL_ADDRESS_TAG_NAME_2 = "addr:full";

PoiPolygonAddressScoreExtractor::PoiPolygonAddressScoreExtractor()
{
}

void PoiPolygonAddressScoreExtractor::setConfiguration(const Settings& conf)
{
  ConfigOptions config = ConfigOptions(conf);
  setExactAddressMatching(config.getPoiPolygonExactAddressMatching());
}

double PoiPolygonAddressScoreExtractor::extract(const OsmMap& map,
                                                const ConstElementPtr& poi,
                                                const ConstElementPtr& poly) const
{
  QStringList polyAddresses;
  double addressScore = -1.0;

  //see if the poly has any address
  _collectAddressesFromElement(poly, polyAddresses);

  if (polyAddresses.size() == 0)
  {
    //if not, try to find the address from a poly way node instead
    if (poly->getElementType() == ElementType::Way)
    {
      _collectAddressesFromWay(dynamic_pointer_cast<const Way>(poly), polyAddresses, map);
    }
    //if still no luck, try to find the address from a poly way node that is a relation member
    else if (poly->getElementType() == ElementType::Relation)
    {
      _collectAddressesFromRelation(dynamic_pointer_cast<const Relation>(poly), polyAddresses, map);
    }
  }
  if (polyAddresses.size() == 0)
  {
    LOG_TRACE("No poly addresses.");
    addressScore = 0.0;
    LOG_VART(addressScore);
    return addressScore;
  }

  //see if the poi has an address
  QStringList poiAddresses;
  _collectAddressesFromElement(poi, poiAddresses);
  if (poiAddresses.size() == 0)
  {
    LOG_TRACE("No POI addresses.");
    addressScore = 0.0;
    LOG_VART(addressScore);
    return addressScore;
  }

  StringDistancePtr addrComp;
  if (_exactAddressMatching)
  {
    addrComp.reset(new ExactStringDistance());
  }
  else
  {
    addrComp.reset(
      new TranslateStringDistance(
        new MeanWordSetDistance(
          new LevenshteinDistance(ConfigOptions().getLevenshteinDistanceAlpha()))));
  }

  double bestAddressScore = -1.0;
  for (int i = 0; i < polyAddresses.size(); i++)
  {
    const QString polyAddress = polyAddresses.at(i);
    for (int j = 0; j < poiAddresses.size(); j++)
    {
      const QString poiAddress = poiAddresses.at(j);
      LOG_VART(poiAddress);
      LOG_VART(polyAddress);
      addressScore = addrComp->compare(polyAddress, poiAddress);
      LOG_VART(addressScore);
      if (addressScore > bestAddressScore)
      {
        bestAddressScore = addressScore;
      }
    }
  }
  if (bestAddressScore > 0.0)
  {
    addressScore = bestAddressScore;
    LOG_VART(addressScore);
    return addressScore;
  }

  addressScore = 0.0;
  LOG_VART(addressScore);
  return addressScore;
}

void PoiPolygonAddressScoreExtractor::_parseAddressesAsRange(const QString houseNum,
                                                             const QString street,
                                                             QStringList& addresses) const
{
  //address ranges; e.g. 1-3 elm street is an address range that includes the addresses:
  //"1 elm street", "2 elm street", and "3 elm street".  I've only seen this on the houseNum
  //tag so far, but conceivably you could find it in the full address tags as well...won't add
  //logic for that, though, unless its encountered.  I could also see someone separating '-'
  //with a space (which isn't hard to handle), but also won't worry about it until its seen
  //in the wild.

  QStringList houseNumParts = houseNum.split("-");
  if (houseNumParts.size() == 2)
  {
    bool startHouseNumParsedOk = false;
    const int startHouseNum = houseNumParts[0].toInt(&startHouseNumParsedOk);
    if (startHouseNumParsedOk)
    {
      bool endHouseNumParsedOk = false;
      const int endHouseNum = houseNumParts[1].toInt(&endHouseNumParsedOk);
      if (endHouseNumParsedOk && startHouseNum < endHouseNum)
      {
        QString combinedAddress;
        for (int i = startHouseNum; i < endHouseNum + 1; i++)
        {
          combinedAddress = QString::number(i) + " " + street;
          LOG_VART(combinedAddress);
          if (!addresses.contains(combinedAddress))
          {
            addresses.append(combinedAddress);
          }
        }
      }
    }
  }
}

void PoiPolygonAddressScoreExtractor::_parseAddressesInAltFormat(const Tags& tags,
                                                                 QStringList& addresses) const
{
  QString addressTagValAltFormatRaw =
    Translator::getInstance().toEnglish(tags.get(FULL_ADDRESS_TAG_NAME_2)).trimmed().toLower();
  if (!addressTagValAltFormatRaw.isEmpty())
  {
    addressTagValAltFormatRaw = addressTagValAltFormatRaw.replace(ESZETT, ESZETT_REPLACE);
    QStringList addressParts = addressTagValAltFormatRaw.split(QRegExp("\\s"));
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
      if (ok && ctr > 1)
      {
        const QString houseNum = addressParts[ctr - 1].replace(QRegExp("[a-z]+"), "").trimmed();
        addressTagValAltFormat += houseNum;
        for (int i = 0; i < (ctr - 1); i++)
        {
          addressTagValAltFormat += " ";
          addressTagValAltFormat += addressParts[i];
        }
        addressTagValAltFormat = addressTagValAltFormat.trimmed();
        LOG_VART(addressTagValAltFormat);
        if (!addresses.contains(addressTagValAltFormat))
        {
          addresses.append(addressTagValAltFormat);
        }
      }
    }
  }
}

void PoiPolygonAddressScoreExtractor::_collectAddressesFromElement(ConstElementPtr element,
                                                                   QStringList& addresses) const
{
  Tags tags = element->getTags();

  //address parts in separate tags (most common situation)
  QString houseNum = tags.get(HOUSE_NUMBER_TAG_NAME).trimmed();
  houseNum = houseNum.replace(QRegExp("[a-z]+"), "");
  QString street =
    Translator::getInstance().toEnglish(tags.get(STREET_TAG_NAME)).trimmed().toLower();
  QString combinedAddress;
  if (!houseNum.isEmpty() && !street.isEmpty())
  {
    //I thought this would have been eliminated by using the translated name comparison
    //logic...seems like it wasn't. - see #1169
    street = street.replace(ESZETT, ESZETT_REPLACE);
    if (!houseNum.contains("-"))
    {
      combinedAddress = houseNum + " " + street;
      if (!addresses.contains(combinedAddress))
      {
        addresses.append(combinedAddress);
      }
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
    if (!addresses.contains(addressTagVal))
    {
      addresses.append(addressTagVal);
    }
  }

  //street name and house num reversed: ZENTRALLÄNDSTRASSE 40 81379 MÜNCHEN
  //parse through the tokens until you come to a number; assume that is the house number and
  //everything before it is the street name
  _parseAddressesInAltFormat(tags, addresses);
}

void PoiPolygonAddressScoreExtractor::_collectAddressesFromWay(ConstWayPtr way,
                                                               QStringList& addresses,
                                                               const OsmMap& map) const
{
  const vector<long> wayNodeIds = way->getNodeIds();
  for (size_t i = 0; i < wayNodeIds.size(); i++)
  {
    _collectAddressesFromElement(map.getElement(ElementType::Node, wayNodeIds.at(i)), addresses);
  }
}

void PoiPolygonAddressScoreExtractor::_collectAddressesFromRelation(ConstRelationPtr relation,
                                                                    QStringList& addresses,
                                                                    const OsmMap& map) const
{
  const vector<RelationData::Entry> relationMembers = relation->getMembers();
  for (size_t i = 0; i < relationMembers.size(); i++)
  {
    ConstElementPtr member = map.getElement(relationMembers[i].getElementId());
    if (member->getElementType() == ElementType::Node)
    {
      _collectAddressesFromElement(member, addresses);
    }
    else if (member->getElementType() == ElementType::Way)
    {
      _collectAddressesFromWay(dynamic_pointer_cast<const Way>(member), addresses, map);
    }
  }
}

}
