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
 * @copyright Copyright (C) 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "PoiPolygonAddressScoreExtractor.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/algorithms/string/MostEnglishName.h>
#include "PoiPolygonAddress.h"

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(FeatureExtractor, PoiPolygonAddressScoreExtractor)

//These seem to be all OSM standard tag names, so don't need to be in a configuration file.
const QString PoiPolygonAddressScoreExtractor::HOUSE_NUMBER_TAG_NAME = "addr:housenumber";
const QString PoiPolygonAddressScoreExtractor::STREET_TAG_NAME = "addr:street";
const QString PoiPolygonAddressScoreExtractor::FULL_ADDRESS_TAG_NAME = "address";
const QString PoiPolygonAddressScoreExtractor::FULL_ADDRESS_TAG_NAME_2 = "addr:full";

boost::shared_ptr<ToEnglishTranslator> PoiPolygonAddressScoreExtractor::_translator;

PoiPolygonAddressScoreExtractor::PoiPolygonAddressScoreExtractor() :
_translateTagValuesToEnglish(false),
_addressesProcessed(0),
_matchAttemptMade(false)
{
}

void PoiPolygonAddressScoreExtractor::setConfiguration(const Settings& conf)
{
  ConfigOptions config = ConfigOptions(conf);
  _translateTagValuesToEnglish = config.getPoiPolygonTranslateAddressesToEnglish();
  if (_translateTagValuesToEnglish && !_translator)
  {
    _translator.reset(
      Factory::getInstance().constructObject<ToEnglishTranslator>(
        config.getLanguageTranslationTranslator()));
    _translator->setConfiguration(conf);
    _translator->setSourceLanguages(config.getLanguageTranslationSourceLanguages());
    _translator->setId(QString::fromStdString(className()));
  }
}

bool PoiPolygonAddressScoreExtractor::isAddressTagKey(const QString tagKey)
{
  return
    tagKey.toLower().startsWith("addr:") || tagKey == FULL_ADDRESS_TAG_NAME;
}

double PoiPolygonAddressScoreExtractor::extract(const OsmMap& map, const ConstElementPtr& poi,
                                                const ConstElementPtr& poly) const
{
  //Experimented with partial addresses matches in the past and it had no positive affect.  Search
  //the history for this class to see examples, to see if its worth experimenting with again at
  //some point.

  QList<PoiPolygonAddress> polyAddresses;
  //see if the poly has any address
  _collectAddressesFromElement(*poly, polyAddresses);
  if (polyAddresses.size() == 0)
  {
    //if not, try to find the address from a poly way node instead
    if (poly->getElementType() == ElementType::Way)
    {
      ConstWayPtr wayPoly = boost::dynamic_pointer_cast<const Way>(poly);
      _collectAddressesFromWayNodes(*wayPoly, polyAddresses, map);
    }
    //if still no luck, try to find the address from a poly way node that is a relation member
    else if (poly->getElementType() == ElementType::Relation)
    {
      ConstRelationPtr relationPoly = boost::dynamic_pointer_cast<const Relation>(poly);
      _collectAddressesFromRelationMembers(*relationPoly, polyAddresses, map);
    }
  }
  if (polyAddresses.size() == 0)
  {
    LOG_TRACE("No poly addresses.");
    return 0.0;
  }

  //see if the poi has an address
  QList<PoiPolygonAddress> poiAddresses;
  _collectAddressesFromElement(*poi, poiAddresses);
  if (poiAddresses.size() == 0)
  {
    LOG_TRACE("No POI addresses.");
    return 0.0;
  }

  _matchAttemptMade = true;
  _addressesProcessed += poiAddresses.size();
  _addressesProcessed += polyAddresses.size();

  //check for address matches
  for (QList<PoiPolygonAddress>::const_iterator polyAddrItr = polyAddresses.begin();
       polyAddrItr != polyAddresses.end(); ++polyAddrItr)
  {
    const PoiPolygonAddress polyAddress = *polyAddrItr;
    for (QList<PoiPolygonAddress>::const_iterator poiAddrItr = poiAddresses.begin();
         poiAddrItr != poiAddresses.end(); ++poiAddrItr)
    {
      const PoiPolygonAddress poiAddress = *poiAddrItr;
      //exact match or subletter fuzziness
      if (polyAddress == poiAddress)
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
                                                          QList<PoiPolygonAddress>& addresses) const
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
    LOG_VART(startHouseNum);
    if (startHouseNumParsedOk)
    {
      bool endHouseNumParsedOk = false;
      const int endHouseNum = houseNumParts[1].toInt(&endHouseNumParsedOk);
      LOG_VART(endHouseNum);
      if (endHouseNumParsedOk && startHouseNum < endHouseNum)
      {
        for (int i = startHouseNum; i < endHouseNum + 1; i++)
        {
          PoiPolygonAddress combinedAddress(QString::number(i) + " " + street);
          if (!addresses.contains(combinedAddress))
          {
            LOG_VART(combinedAddress);
            addresses.append(combinedAddress);
          }
        }
      }
    }
  }
}

void PoiPolygonAddressScoreExtractor::_parseAddressesInAltFormat(const Tags& tags,
                                                         QList<PoiPolygonAddress>& addresses) const
{
  //street name and house num reversed: ZENTRALLÄNDSTRASSE 40 81379 MÜNCHEN
  //parse through the tokens until you come to a number; assume that is the house number and
  //everything before it is the street name

  //This may be able to be cleaned up with regex's.

  QString addressTagValAltFormatRaw = tags.get(FULL_ADDRESS_TAG_NAME_2).trimmed();
  if (!addressTagValAltFormatRaw.isEmpty())
  {
    addressTagValAltFormatRaw = addressTagValAltFormatRaw.toLower();
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
        const QString houseNum = addressParts[ctr - 1];
        addressTagValAltFormat += houseNum;
        for (int i = 0; i < (ctr - 1); i++)
        {
          addressTagValAltFormat += " ";
          addressTagValAltFormat += addressParts[i];
        }
        addressTagValAltFormat = addressTagValAltFormat.trimmed();
        LOG_VART(addressTagValAltFormat);
        QString translatedAddressTagValAltFormat = addressTagValAltFormat;
        if (_translateTagValuesToEnglish)
        {
          _translateAddressToEnglish(translatedAddressTagValAltFormat);
        }
        addresses.append(
          PoiPolygonAddress(addressTagValAltFormat, translatedAddressTagValAltFormat));
      }
    }
  }
}

bool PoiPolygonAddressScoreExtractor::nodeHasAddress(const Node& node)
{
  PoiPolygonAddressScoreExtractor addressExtractor;
  // We're just getting a count here, so translation isn't needed (can be expensive).
  addressExtractor._translateTagValuesToEnglish = false;
  QList<PoiPolygonAddress> addresses;
  addressExtractor._collectAddressesFromElement(node, addresses);
  return addresses.size() > 0;
}

bool PoiPolygonAddressScoreExtractor::elementHasAddress(const ConstElementPtr& element,
                                                        const OsmMap& map)
{
  PoiPolygonAddressScoreExtractor addressExtractor;
  // We're just getting a count here, so translation isn't needed (can be expensive).
  addressExtractor._translateTagValuesToEnglish = false;
  QList<PoiPolygonAddress> addresses;
  if (element->getElementType() == ElementType::Node)
  {
    return nodeHasAddress(*boost::dynamic_pointer_cast<const Node>(element));
  }
  else if (element->getElementType() == ElementType::Way)
  {
    addressExtractor._collectAddressesFromWayNodes(
      *boost::dynamic_pointer_cast<const Way>(element), addresses, map);
  }
  else if (element->getElementType() == ElementType::Relation)
  {
    addressExtractor._collectAddressesFromRelationMembers(
      *boost::dynamic_pointer_cast<const Relation>(element), addresses, map);
  }
  return addresses.size() > 0;
}

void PoiPolygonAddressScoreExtractor::_translateAddressToEnglish(QString& address) const
{
  const QStringList addressParts = address.simplified().split(" ");
  //Try to translate blocks of consecutive address tokens to cut down on the number of
  //translation calls made.
  QString combinedAddressPartToTranslate = "";
  QStringList combinedAddressPartsToTranslate;
  for (int i = 0; i < addressParts.size(); i++)
  {
    const QString addressPart = addressParts.at(i);
    LOG_VART(addressPart);
    if (!StringUtils::isNumber(addressPart))
    {
      combinedAddressPartToTranslate += " " + addressPart;
      LOG_VART(combinedAddressPartToTranslate);
    }
    else if (!combinedAddressPartToTranslate.isEmpty())
    {
      combinedAddressPartsToTranslate.append(combinedAddressPartToTranslate.trimmed());
      combinedAddressPartToTranslate = "";
    }
    LOG_VART(combinedAddressPartsToTranslate);
  }
  if (!combinedAddressPartToTranslate.isEmpty())
  {
    combinedAddressPartsToTranslate.append(combinedAddressPartToTranslate.trimmed());
  }
  LOG_VART(combinedAddressPartsToTranslate);

  bool anyAddressPartWasTranslated = false;
  QString translatedAddress = address;
  for (int i = 0; i < combinedAddressPartsToTranslate.size(); i++)
  {
    const QString combinedAddressPart = combinedAddressPartsToTranslate.at(i).trimmed();
    LOG_VART(combinedAddressPart);
    const QString translatedCombinedAddressPart = _translator->translate(combinedAddressPart);
    if (!translatedCombinedAddressPart.isEmpty())
    {
      translatedAddress =
        translatedAddress.replace(combinedAddressPart, translatedCombinedAddressPart);
      LOG_VART(translatedAddress);
      anyAddressPartWasTranslated = true;
    }
  }

  if (anyAddressPartWasTranslated)
  {
    LOG_TRACE("Translated address from " << address << " to " << translatedAddress);
    address = translatedAddress;
  }
  else
  {
    LOG_TRACE("Address " << address << " could not be translated.");
    address = "";
  }
}

void PoiPolygonAddressScoreExtractor::_collectAddressesFromElement(const Element& element,
                                                          QList<PoiPolygonAddress>& addresses) const
{
  //We're parsing multiple types of address tags here, b/c its possible that the same feature
  //could have multiple types of address tags with only one of them being accurate.  Parsing just
  //one type does the same on run time, but can be less accurate for the aforementioned reason.
  //This code assumes that only one type of address will be present on each element.  If that
  //assumption ever proves false, then this needs to be reworked to throw an error or process only
  //the first address type found.

  const Tags tags = element.getTags();

  //address parts in separate tags (most common situation)
  QString houseNum = tags.get(HOUSE_NUMBER_TAG_NAME).trimmed();
  LOG_VART(houseNum);
  QString street = tags.get(STREET_TAG_NAME).trimmed();
  LOG_VART(street);
  if (!houseNum.isEmpty() && !street.isEmpty())
  {
    QString translatedStreet = street;
    if (_translateTagValuesToEnglish)
    {
      _translateAddressToEnglish(translatedStreet);
    }
    street = street.toLower();
    houseNum = houseNum.replace(QRegExp("[a-z]+"), "");
    LOG_VART(houseNum);
    LOG_VART(street);
    if (!houseNum.contains("-"))
    {
      const QString combinedAddress = houseNum + " " + street;
      LOG_VART(combinedAddress);
      const QString combinedTranslatedAddress = houseNum + " " + translatedStreet;
      LOG_VART(combinedTranslatedAddress);
      addresses.append(PoiPolygonAddress(combinedAddress, combinedTranslatedAddress));
    }
    else
    {
      _parseAddressesAsRange(houseNum, street, addresses);
    }
  }

  //full address in one tag
  QString addressTagVal = tags.get(FULL_ADDRESS_TAG_NAME).trimmed();
  LOG_VART(addressTagVal);
  if (!addressTagVal.isEmpty())
  {
    QString translatedAddressTagVal = addressTagVal;
    if (_translateTagValuesToEnglish)
    {
      _translateAddressToEnglish(translatedAddressTagVal);
    }
    addressTagVal = addressTagVal.toLower();
    LOG_VART(addressTagVal);
    translatedAddressTagVal = translatedAddressTagVal.toLower();
    LOG_VART(translatedAddressTagVal);
    addresses.append(PoiPolygonAddress(addressTagVal, translatedAddressTagVal));
  }

  _parseAddressesInAltFormat(tags, addresses);
}

void PoiPolygonAddressScoreExtractor::_collectAddressesFromWayNodes(const Way& way,
                                                                    QList<PoiPolygonAddress>& addresses,
                                                                    const OsmMap& map) const
{
  const vector<long> wayNodeIds = way.getNodeIds();
  for (size_t i = 0; i < wayNodeIds.size(); i++)
  {
    _collectAddressesFromElement(*(map.getElement(ElementType::Node, wayNodeIds.at(i))), addresses);
  }
}

void PoiPolygonAddressScoreExtractor::_collectAddressesFromRelationMembers(const Relation& relation,
                                                                           QList<PoiPolygonAddress>& addresses,
                                                                           const OsmMap& map) const
{
  const vector<RelationData::Entry> relationMembers = relation.getMembers();
  for (size_t i = 0; i < relationMembers.size(); i++)
  {
    ConstElementPtr member = map.getElement(relationMembers[i].getElementId());
    if (member->getElementType() == ElementType::Node)
    {
      _collectAddressesFromElement(*member, addresses);
    }
    else if (member->getElementType() == ElementType::Way)
    {
      ConstWayPtr wayMember = boost::dynamic_pointer_cast<const Way>(member);
      _collectAddressesFromWayNodes(*wayMember, addresses, map);
    }
  }
}

}
