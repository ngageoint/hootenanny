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
 * @copyright Copyright (C) 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "AddressParser.h"

// hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/conflate/address/LibPostalInit.h>
#include <hoot/core/algorithms/string/ExactStringDistance.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/conflate/address/Address.h>
#include <hoot/core/util/StringUtils.h>

// libpostal
#include <libpostal/libpostal.h>

namespace hoot
{

AddressParser::AddressParser() :
_allowLenientHouseNumberMatching(true),
_preTranslateTagValuesToEnglish(false)
{
}

void AddressParser::setConfiguration(const Settings& conf)
{
  setPreTranslateTagValuesToEnglish(_preTranslateTagValuesToEnglish, conf);
}

void AddressParser::setPreTranslateTagValuesToEnglish(bool translate, const Settings& conf)
{
  _preTranslateTagValuesToEnglish = translate;
  if (_preTranslateTagValuesToEnglish)
  {
    _addressTranslator.setConfiguration(conf);
  }
}

bool AddressParser::hasAddress(const Element& element) const
{
  return numAddresses(element) > 0;
}

int AddressParser::numAddresses(const Element& element) const
{
  // We're just getting a count here, so translation isn't needed (can be expensive).
  bool translateModified = false;
  if (_preTranslateTagValuesToEnglish)
  {
    translateModified = true;
    _preTranslateTagValuesToEnglish = false;
  }
  // Normalization will sometimes result in some false positive additional addresses, which doesn't
  // matter that much when doing address comparison, but does matter when trying to get an accurate
  // count for address tags.
  const int numAddresses = parseAddresses(element, false).size();
  if (translateModified)
  {
    _preTranslateTagValuesToEnglish = true;
  }
  return numAddresses;
}

bool AddressParser::hasAddressRecursive(const ConstElementPtr& element, const OsmMap& map) const
{
  return numAddressesRecursive(element, map) > 0;
}

int AddressParser::numAddressesRecursive(const ConstElementPtr& element, const OsmMap& map) const
{
  // We're just getting a count here, so translation isn't needed (can be expensive).
  bool translateModified = false;
  if (_preTranslateTagValuesToEnglish)
  {
    translateModified = true;
    _preTranslateTagValuesToEnglish = false;
  }
  QList<Address> addresses;
  if (element->getElementType() == ElementType::Node)
  {
    return numAddresses(*std::dynamic_pointer_cast<const Node>(element));
  }
  else if (element->getElementType() == ElementType::Way)
  {
    addresses = parseAddressesFromWayNodes(*std::dynamic_pointer_cast<const Way>(element), map);
    // TODO: Uncommenting this blows up a few of the poi/poly regression tests by forcing some
    // address comparisons. See #3627.
    //addresses.append(parseAddresses(*element));
  }
  else if (element->getElementType() == ElementType::Relation)
  {
    addresses =
      parseAddressesFromRelationMembers(*std::dynamic_pointer_cast<const Relation>(element), map);
    addresses.append(parseAddresses(*element));
  }
  if (translateModified)
  {
    _preTranslateTagValuesToEnglish = true;
  }
  return addresses.size();
}

QList<Address> AddressParser::parseAddresses(const Element& element,
                                             const bool normalizeAddresses) const
{
  // Make this call here, so that we don't cause it to be done unnecessarily as part of this
  // class's init when its a mem var on another class, since this init is expensive.
  LibPostalInit::getInstance();

  LOG_VART(element.getElementId());
  QList<Address> addresses;

  QString houseNum;
  QString street;
  const QSet<QString> parsedAddresses = _parseAddresses(element, houseNum, street);
  LOG_VART(parsedAddresses);

  // add the parsed addresses to a collection in which they will later be compared to each other
  for (QSet<QString>::const_iterator parsedAddressItr = parsedAddresses.begin();
       parsedAddressItr != parsedAddresses.end(); ++parsedAddressItr)
  {
    QString parsedAddress = *parsedAddressItr;
    LOG_VART(parsedAddress);

    //optional additional lang pre-normalization translation
    if (_preTranslateTagValuesToEnglish)
    {
      //only translating the street portion of the address string...the number isn't translatable
       assert(!street.isEmpty());
      const QString preTranslatedStreet = _addressTranslator.translateToEnglish(street);
      if (!preTranslatedStreet.isEmpty())
      {
        parsedAddress = houseNum + " " + preTranslatedStreet;
      }
      else
      {
        parsedAddress = houseNum + " " + street;
      }
    }
    LOG_VART(parsedAddress);

    if (normalizeAddresses)
    {
      // normalize and translate the address strings, so we end up comparing apples to apples
      const QSet<QString> normalizedAddresses = _addressNormalizer.normalizeAddress(parsedAddress);
      LOG_VART(normalizedAddresses);

      for (QSet<QString>::const_iterator normalizedAddressItr = normalizedAddresses.begin();
           normalizedAddressItr != normalizedAddresses.end(); ++normalizedAddressItr)
      {
        const QString normalizedAddress = *normalizedAddressItr;
        LOG_VART(normalizedAddress);
        Address address(normalizedAddress, _allowLenientHouseNumberMatching);
        if (!addresses.contains(address))
        {
          LOG_TRACE("Adding address: " << address << " for element: " << element.getElementId());
          addresses.append(address);
        }
      }
    }
    else
    {
      Address address(parsedAddress, _allowLenientHouseNumberMatching);
      if (!addresses.contains(address))
      {
        LOG_TRACE("Adding address: " << address << " for element: " << element.getElementId());
        addresses.append(address);
      }
    }
  }

  return addresses;
}

QList<Address> AddressParser::parseAddressesFromWayNodes(const Way& way, const OsmMap& map,
                                                         const ElementId& skipElementId) const
{
  QList<Address> addresses;
  LOG_TRACE("Collecting addresses from way nodes...");
  const std::vector<long> wayNodeIds = way.getNodeIds();
  for (size_t i = 0; i < wayNodeIds.size(); i++)
  {
    ConstElementPtr wayNode = map.getElement(ElementType::Node, wayNodeIds.at(i));
    // see explanation for this check in parseAddressesFromRelationMembers
    if (skipElementId.isNull() || wayNode->getElementId() != skipElementId)
    {
      addresses = parseAddresses(*wayNode);
    }
  }
  return addresses;
}

QList<Address> AddressParser::parseAddressesFromRelationMembers(const Relation& relation,
                                                                const OsmMap& map,
                                                                const ElementId& skipElementId) const
{
  QList<Address> addresses;
  LOG_TRACE("Collecting addresses from relation members...");
  const std::vector<RelationData::Entry> relationMembers = relation.getMembers();
  for (size_t i = 0; i < relationMembers.size(); i++)
  {
    ConstElementPtr member = map.getElement(relationMembers[i].getElementId());
    // If the poly contains the poi being compared to as a way node or relation member, then the
    // POI's address will be added to both the poly and poi group of addresses and yield a fake
    // address match
    if (skipElementId.isNull() || member->getElementId() != skipElementId)
    {
      if (member->getElementType() == ElementType::Node)
      {
        addresses = parseAddresses(*member);
      }
      else if (member->getElementType() == ElementType::Way)
      {
        ConstWayPtr wayMember = std::dynamic_pointer_cast<const Way>(member);
        addresses = parseAddressesFromWayNodes(*wayMember, map, skipElementId);
        addresses.append(parseAddresses(*wayMember));
      }
    }
  }
  return addresses;
}

QSet<QString> AddressParser::_parseAddressAsRange(const QString& houseNum,
                                                  const QString& street) const
{
  //address ranges; e.g. 1-3 elm street is an address range that includes the addresses:
  //"1 elm street", "2 elm street", and "3 elm street".  I've only seen this on the houseNum
  //tag so far, but conceivably you could find it in the full address tags as well...won't add
  //logic for that, though, unless its encountered.  I could also see someone separating '-'
  //with a space (which isn't hard to handle), but also won't worry about it until its seen
  //in the wild.

  //This may be able to be cleaned up some with regex's.

  QSet<QString> parsedAddresses;
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
          const QString parsedAddress = QString::number(i) + " " + street;
          LOG_TRACE("Parsed address as range: " << parsedAddress);
          parsedAddresses.insert(parsedAddress);
        }
      }
    }
  }
  return parsedAddresses;
}

bool AddressParser::_isParseableAddressFromComponents(const Tags& tags, QString& houseNum,
                                                      QString& street) const
{
  // we only require a valid street address...no other higher order parts, like city, state, etc.
  houseNum = AddressTagKeys::getInstance().getAddressTagValue(tags, "house_number");
  street = AddressTagKeys::getInstance().getAddressTagValue(tags, "street").toLower();
  if (!houseNum.isEmpty() && !street.isEmpty())
  {
    LOG_TRACE("Found address from components: " << houseNum << ", " << street << ".");
    return true;
  }
  return false;
}

bool AddressParser::_isRangeAddress(const QString& houseNum) const
{
  return houseNum.contains("-");
}

bool AddressParser::_isValidAddressStr(QString& address, QString& houseNum, QString& street,
                                       const bool requireStreetTypeInIntersection) const
{
  // use libpostal to break down the address string
  libpostal_address_parser_response_t* parsed =
    libpostal_parse_address(
      address.toUtf8().data(), libpostal_get_address_parser_default_options());
  /*
    Label: house_number, Component: 781
    Label: road, Component: franklin ave
    Label: suburb, Component: crown heights
    Label: city_district, Component: brooklyn
    Label: city, Component: nyc
    Label: state, Component: ny
    Label: postcode, Component: 11216
    Label: country, Component: usa
   */
  for (size_t i = 0; i < parsed->num_components; i++)
  {
    const QString label = parsed->labels[i];
    LOG_VART(label);
    const QString component = QString::fromUtf8((const char*)parsed->components[i]);
    LOG_VART(component);
    // we only care about the street address
    if (label == "house_number")
    {
      houseNum = component;
    }
    else if (label == "road")
    {
      street = component;
    }
  }
  libpostal_address_parser_response_destroy(parsed);
  LOG_VART(street);
  LOG_VART(houseNum);
  LOG_VART(requireStreetTypeInIntersection);
  LOG_VART(Address::isIntersectionAddress(street, requireStreetTypeInIntersection));

  if (!houseNum.isEmpty() && !street.isEmpty())
  {
    address = houseNum + " " + street;
    address = address.trimmed();
    LOG_TRACE("Found address: " << address);
    return true;
  }
  // intersections won't have numbers
  else if (Address::isIntersectionAddress(street, requireStreetTypeInIntersection))
  {
    address = street;
    address = address.trimmed();
    LOG_TRACE("Found intersection address: " << address);
    return true;
  }
  else
  {
    return false;
  }
}

QString AddressParser::_parseFullAddress(const QString& fullAddress, QString& houseNum,
                                         QString& street) const
{
  QString parsedAddress = fullAddress;

  QString msg = "Found";
  // validate the full address
  bool validAddress = false;
  if (_isValidAddressStr(parsedAddress, houseNum, street))
  {
    validAddress = true;
    msg += " valid";
  }
  else
  {
    msg += " invalid";
  }
  msg += " address: " + parsedAddress + " from full address: " + fullAddress;
  LOG_TRACE(msg);

  if (validAddress)
  {
    return parsedAddress;
  }
  return "";
}

QSet<QString> AddressParser::_parseAddressFromComponents(const Tags& tags, QString& houseNum,
                                                         QString& street) const
{
  LOG_TRACE("Parsing address from component tags...");
  QSet<QString> parsedAddresses;

  if (_isParseableAddressFromComponents(tags, houseNum, street))
  {
    if (_isRangeAddress(houseNum))
    {
      // use custom logic for a address containing a range of addresses
      parsedAddresses = _parseAddressAsRange(houseNum, street);
    }
    else
    {
      QString parsedAddress = houseNum + " ";
      const QString streetPrefix =
        AddressTagKeys::getInstance().getAddressTagValue(tags, "street_prefix");
      if (!streetPrefix.isEmpty())
      {
        parsedAddress += streetPrefix + " ";
      }
      parsedAddress += street;
      const QString streetSuffix =
        AddressTagKeys::getInstance().getAddressTagValue(tags, "street_suffix");
      if (!streetSuffix.isEmpty())
      {
        parsedAddress += " " + streetPrefix;
      }
      parsedAddresses.insert(parsedAddress);
      LOG_TRACE(
        "Found address by parsing address parts from component tags: " << parsedAddress << ".");
    }
  }

  return parsedAddresses;
}

QString AddressParser::_parseAddressFromAltTags(const Tags& tags, QString& houseNum,
                                                QString& street) const
{
  LOG_TRACE("Parsing address from alt tags...");
  QString parsedAddress;

  //let's always look in the name field; arguably, we could look in all of them instead of just
  //one...
  QSet<QString> additionalTagKeys = AddressTagKeys::getInstance().getAdditionalTagKeys();
  LOG_VART(additionalTagKeys);

  for (QSet<QString>::const_iterator tagItr = additionalTagKeys.begin();
       tagItr != additionalTagKeys.end(); ++tagItr)
  {
    const QString tagKey = *tagItr;
    QString tagVal = tags.get(tagKey);
    if (!tagVal.isEmpty() && _isValidAddressStr(tagVal, houseNum, street))
    {
      parsedAddress = tagVal;
      LOG_TRACE("Found address: " << parsedAddress << " from additional tag key: " << tagKey << ".");
      break;
    }
  }

  additionalTagKeys = QSet<QString>::fromList(tags.getNameKeys());
  LOG_VART(additionalTagKeys);
  for (QSet<QString>::const_iterator tagItr = additionalTagKeys.begin();
       tagItr != additionalTagKeys.end(); ++tagItr)
  {
    const QString tagKey = *tagItr;
    QString tagVal = tags.get(tagKey);
    if (!tagVal.isEmpty() && _isValidAddressStr(tagVal, houseNum, street, true))
    {
      parsedAddress = tagVal;
      LOG_TRACE("Found address: " << parsedAddress << " from additional tag key: " << tagKey << ".");
      break;
    }
  }

  return parsedAddress;
}

QSet<QString> AddressParser::_parseAddresses(const Element& element, QString& houseNum,
                                             QString& street) const
{
  // We're going to take the most likely correct address and ignore any others.
  QSet<QString> parsedAddresses;

  // look for a full address tag first
  QString fullAddress =
    AddressTagKeys::getInstance().getAddressTagValue(element.getTags(), "full_address");
  LOG_VART(fullAddress);
  if (fullAddress.isEmpty())
  {
    // if we don't have the full address, let's try to get an address from parts
    parsedAddresses = _parseAddressFromComponents(element.getTags(), houseNum, street);
  }
  else
  {
    // if we do have a full address, let's parse and validate it
    const QString parsedAddress = _parseFullAddress(fullAddress, houseNum, street);
    if (!parsedAddress.isEmpty())
    {
      parsedAddresses.insert(parsedAddress);
    }
  }

  if (parsedAddresses.isEmpty())
  {
    // we didn't find anything in the standard address tags, so let's try feature names and any user
    // defined tags
    const QString parsedAddress = _parseAddressFromAltTags(element.getTags(), houseNum, street);
    if (!parsedAddress.isEmpty())
    {
      parsedAddresses.insert(parsedAddress);
    }
  }

  return parsedAddresses;
}

bool AddressParser::addressesMatchDespiteSubletterDiffs(const QString& address1,
                                                        const QString& address2)
{
  LOG_VART(address1);
  LOG_VART(address2);

  /* going to allow sub letter differences be matches; ex "34 elm street" matches
   * "34a elm street".  This is b/c the subletters are sometimes left out of the addresses by
   * accident, and we'd like to at least end up with a review in that situation.
   */

  const QString subletterCleanedAddress1 = _getSubLetterCleanedAddress(address1);
  const QString subletterCleanedAddress2 = _getSubLetterCleanedAddress(address2);
  if (!subletterCleanedAddress1.isEmpty() && !subletterCleanedAddress2.isEmpty() &&
      ExactStringDistance().compare(subletterCleanedAddress1, subletterCleanedAddress2) == 1.0)
  {
    return true;
  }

  return false;
}

QString AddressParser::_getSubLetterCleanedAddress(const QString& address)
{
  //may be able to clean some of this up with better use of regex's

  const QStringList addressParts = address.split(QRegExp("\\s"));
  if (addressParts.length() == 0)
  {
    return "";
  }

  QString addressTemp = addressParts[0];
  const QString addressHouseNumStr = addressTemp.replace(QRegExp("[a-z]+"), "");
  LOG_VART(addressHouseNumStr);
  bool addressHouseNumOk = false;
  /*const int address1HouseNum = */addressHouseNumStr.toInt(&addressHouseNumOk);

  if (!addressHouseNumOk)
  {
    return "";
  }

  QString subletterCleanedAddress = addressHouseNumStr;
  for (int k = 1; k < addressParts.length(); k++)
  {
    subletterCleanedAddress += " " + addressParts[k];
  }
  LOG_VART(subletterCleanedAddress);
  return subletterCleanedAddress;
}

}
