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
//#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(FeatureExtractor, PoiPolygonAddressScoreExtractor)

const QChar PoiPolygonAddressScoreExtractor::ESZETT(0x00DF);
const QString PoiPolygonAddressScoreExtractor::ESZETT_REPLACE = "ss";

//These seem to be all OSM standard tag names, so don't need to be in a configuration file.
const QString PoiPolygonAddressScoreExtractor::HOUSE_NUMBER_TAG_NAME = "addr:housenumber";
const QString PoiPolygonAddressScoreExtractor::STREET_TAG_NAME = "addr:street";
const QString PoiPolygonAddressScoreExtractor::FULL_ADDRESS_TAG_NAME = "address";
const QString PoiPolygonAddressScoreExtractor::FULL_ADDRESS_TAG_NAME_2 = "addr:full";

PoiPolygonAddressScoreExtractor::PoiPolygonAddressScoreExtractor()
{
}

//void PoiPolygonAddressScoreExtractor::setConfiguration(const Settings& conf)
//{
//  ConfigOptions config = ConfigOptions(conf);
//  setAddressScoreThreshold(config.getPoiPolygonAddressScoreThreshold());
//}

double PoiPolygonAddressScoreExtractor::extract(const OsmMap& map, const ConstElementPtr& poi,
                                                const ConstElementPtr& poly) const
{
  //Experimented with partial addresses matches in the past and it had no positive affect.  Search
  //the history for this class to see examples if its worth experimenting with again at some point.

  QSet<QString> polyAddresses;

  //see if the poly has any address
  _collectAddressesFromElement(poly, polyAddresses);

  if (polyAddresses.size() == 0)
  {
    //if not, try to find the address from a poly way node instead
    if (poly->getElementType() == ElementType::Way)
    {
      _collectAddressesFromWayNodes(dynamic_pointer_cast<const Way>(poly), polyAddresses, map);
    }
    //if still no luck, try to find the address from a poly way node that is a relation member
    else if (poly->getElementType() == ElementType::Relation)
    {
      _collectAddressesFromRelationMembers(
        dynamic_pointer_cast<const Relation>(poly), polyAddresses, map);
    }
  }
  if (polyAddresses.size() == 0)
  {
    LOG_TRACE("No poly addresses.");
    return 0.0;
  }

  //see if the poi has an address
  QSet<QString> poiAddresses;
  _collectAddressesFromElement(poi, poiAddresses);
  if (poiAddresses.size() == 0)
  {
    LOG_TRACE("No POI addresses.");
    return 0.0;
  }

  ExactStringDistance addrComp;
  for (int i = 0; i < polyAddresses.size(); i++)
  for (QSet<QString>::const_iterator polyAddrItr = polyAddresses.begin();
       polyAddrItr != polyAddresses.end(); ++polyAddrItr)
  {
    const QString polyAddress = *polyAddrItr;
    for (QSet<QString>::const_iterator poiAddrItr = poiAddresses.begin();
         poiAddrItr != poiAddresses.end(); ++poiAddrItr)
    {
      const QString poiAddress = *poiAddrItr;
      //exact match
      if (addrComp.compare(polyAddress, poiAddress) == 1.0)
      {
        LOG_TRACE("Found address match.");
        return 1.0;
      }
      //subletter fuzziness
      else if (_addressesMatchesOnSubLetter(polyAddress, poiAddress))
      {
        LOG_TRACE("Found address match.");
        return 1.0;
      }
    }
  }

  return 0.0;
}

void PoiPolygonAddressScoreExtractor::_parseAddressesAsRange(const QString houseNum,
                                                             const QString street,
                                                             QSet<QString>& addresses) const
{
  //address ranges; e.g. 1-3 elm street is an address range that includes the addresses:
  //"1 elm street", "2 elm street", and "3 elm street".  I've only seen this on the houseNum
  //tag so far, but conceivably you could find it in the full address tags as well...won't add
  //logic for that, though, unless its encountered.  I could also see someone separating '-'
  //with a space (which isn't hard to handle), but also won't worry about it until its seen
  //in the wild.

  //This may be able to be cleaned up with regex's.

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
          if (!addresses.contains(combinedAddress))
          {
            LOG_VART(combinedAddress);
            addresses.insert(combinedAddress);
          }
        }
      }
    }
  }
}

void PoiPolygonAddressScoreExtractor::_parseAddressesInAltFormat(const Tags& tags,
                                                                 QSet<QString>& addresses) const
{
  //street name and house num reversed: ZENTRALLÄNDSTRASSE 40 81379 MÜNCHEN
  //parse through the tokens until you come to a number; assume that is the house number and
  //everything before it is the street name

  //This may be able to be cleaned up with regex's.

  QString addressTagValAltFormatRaw = tags.get(FULL_ADDRESS_TAG_NAME_2).trimmed();
  if (!addressTagValAltFormatRaw.isEmpty())
  {
    addressTagValAltFormatRaw =
      Translator::getInstance().toEnglish(addressTagValAltFormatRaw).toLower();
    addressTagValAltFormatRaw = addressTagValAltFormatRaw.replace(ESZETT, ESZETT_REPLACE);
    const QStringList addressParts = addressTagValAltFormatRaw.split(QRegExp("\\s"));
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
        const QString houseNum = addressParts[ctr - 1]/*.replace(QRegExp("[a-z]+"), "").trimmed()*/;
        addressTagValAltFormat += houseNum;
        for (int i = 0; i < (ctr - 1); i++)
        {
          addressTagValAltFormat += " ";
          addressTagValAltFormat += addressParts[i];
        }
        addressTagValAltFormat = addressTagValAltFormat.trimmed();
        LOG_VART(addressTagValAltFormat);
        addresses.insert(addressTagValAltFormat);
      }
    }
  }
}

bool PoiPolygonAddressScoreExtractor::_addressesMatchesOnSubLetter(const QString polyAddress,
                                                                   const QString poiAddress) const
{
  /* we're also going to allow sub letter differences be matches; ex "34 elm street" matches
   * "34a elm street".  This is b/c the subletters are sometimes left out of the addresses by
   * accident, and we'd like to at least end up with a review in that situation.
   */

  //a lot in here may be able to be cleaned up with better use of regex's
  const QStringList polyAddressParts = polyAddress.split(QRegExp("\\s"));
  if (polyAddressParts.length() == 0)
  {
    return false;
  }
  const QStringList poiAddressParts = poiAddress.split(QRegExp("\\s"));
  if (poiAddressParts.length() == 0)
  {
    return false;
  }

  QString polyAddressTemp = polyAddressParts[0];
  const QString polyHouseNumStr = polyAddressTemp.replace(QRegExp("[a-z]+"), "");
  LOG_VARD(polyHouseNumStr);
  bool polyHouseNumOk = false;
  /*const int polyHouseNum = */polyHouseNumStr.toInt(&polyHouseNumOk);

  QString poiAddressTemp = poiAddressParts[0];
  const QString poiHouseNumStr = poiAddressTemp.replace(QRegExp("[a-z]+"), "");
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
    LOG_VART(subletterCleanedPolyAddress);

    QString subletterCleanedPoiAddress = poiHouseNumStr;
    for (int k = 1; k < poiAddressParts.length(); k++)
    {
      subletterCleanedPoiAddress += " " + poiAddressParts[k];
    }
    LOG_VART(subletterCleanedPoiAddress);

    ExactStringDistance addrComp;
    if (addrComp.compare(subletterCleanedPolyAddress, subletterCleanedPoiAddress) == 1.0)
    {
      return true;
    }
  }

  return false;
}

void PoiPolygonAddressScoreExtractor::_collectAddressesFromElement(ConstElementPtr element,
                                                                   QSet<QString>& addresses) const
{
  //We're parsing multiple types of address tags here, b/c its possible that the same feature
  //could have multiple types of address tags with only one of them being accurate.  Parsing just
  //one type does same on run time, but can be less accurate for the aforementioned reason.

  const Tags tags = element->getTags();

  //address parts in separate tags (most common situation)
  QString houseNum = tags.get(HOUSE_NUMBER_TAG_NAME).trimmed();
  QString street = tags.get(STREET_TAG_NAME).trimmed();
  if (!houseNum.isEmpty() && !street.isEmpty())
  {
    street = Translator::getInstance().toEnglish(street).toLower();
    QString combinedAddress;
    houseNum = houseNum.replace(QRegExp("[a-z]+"), "");
    //hack - I thought this would have been eliminated by using the translated name comparison
    //logic...seems like it wasn't. - see others
    street = street.replace(ESZETT, ESZETT_REPLACE);
    if (!houseNum.contains("-"))
    {
      combinedAddress = houseNum + " " + street;
      addresses.insert(combinedAddress);
    }
    else
    {
      _parseAddressesAsRange(houseNum, street, addresses);
    }
  }

  //full address in one tag
  QString addressTagVal = tags.get(FULL_ADDRESS_TAG_NAME).trimmed();
  if (!addressTagVal.isEmpty())
  {
    addressTagVal = Translator::getInstance().toEnglish(addressTagVal).toLower();
    addressTagVal = addressTagVal.replace(ESZETT, ESZETT_REPLACE);
    addresses.insert(addressTagVal);
  }

  //street name and house num reversed: ZENTRALLÄNDSTRASSE 40 81379 MÜNCHEN
  //parse through the tokens until you come to a number; assume that is the house number and
  //everything before it is the street name
  _parseAddressesInAltFormat(tags, addresses);
}

void PoiPolygonAddressScoreExtractor::_collectAddressesFromWayNodes(ConstWayPtr way,
                                                                    QSet<QString>& addresses,
                                                                    const OsmMap& map) const
{
  const vector<long> wayNodeIds = way->getNodeIds();
  for (size_t i = 0; i < wayNodeIds.size(); i++)
  {
    _collectAddressesFromElement(map.getElement(ElementType::Node, wayNodeIds.at(i)), addresses);
  }
}

void PoiPolygonAddressScoreExtractor::_collectAddressesFromRelationMembers(ConstRelationPtr relation,
                                                                           QSet<QString>& addresses,
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
      _collectAddressesFromWayNodes(dynamic_pointer_cast<const Way>(member), addresses, map);
    }
  }
}

}
