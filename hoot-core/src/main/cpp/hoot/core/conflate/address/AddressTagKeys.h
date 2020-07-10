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
#ifndef ADDRESS_TAG_KEYS_H
#define ADDRESS_TAG_KEYS_H

// hoot
#include <hoot/core/elements/Element.h>

// Qt
#include <QMultiMap>
#include <QSet>

namespace hoot
{

class AddressTagKeys;

typedef std::shared_ptr<AddressTagKeys> AddressTagKeysPtr;

/**
 * Allows for mapping an address part type to a range of valid OSM tag keys
 */
class AddressTagKeys
{
public:

  AddressTagKeys();

  /**
   * Returns the tag keys of all address tags on an element
   *
   * @param element the element to parse tag keys from
   * @return a collection of tag keys
   */
  QSet<QString> getAddressTagKeys(const Element& element) const;

  /**
   * Returns a tag key of the specified address type from a set of tags
   *
   * @param tags a set of element tags
   * @param addressTagType the type of address tag to retrieve; see the contents of the file pointed
   * to by the configuration setting, address.tag.keys.file, for more detail
   * @return an address tag key or an empty string if no address tag of the specified type is found
   */
  QString getAddressTagKey(const Tags& tags, const QString& addressTagType) const;

  /**
   * Returns a tag value of the specified address type from a set of tags
   *
   * @param tags a set of element tags
   * @param addressTagType the type of address tag to retrieve; see the contents of the file pointed
   * to by the configuration setting, address.tag.keys.file, for more detail
   * @return an address tag value or an empty string if no address tag of the specified type is
   * found
   */
  QString getAddressTagValue(const Tags& tags, const QString& addressTagType) const;

  QSet<QString> getAdditionalTagKeys() const { return _additionalTagKeys; }

private:

  /*
   * Reads tag keys used to identify tags as addresses
   */
  void _readAddressTagKeys(const QString& configFile);

  QString _getAddressTag(const Tags& tags, const QString& addressTagType, bool key) const;

  // extra tags to search for addresses in
  QSet<QString> _additionalTagKeys;
  // maps address tag types to valid address tag keys
  static QMultiMap<QString, QString> _addressTypeToTagKeys;
};

}

#endif // ADDRESS_TAG_KEYS_H
