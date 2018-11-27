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
 * @copyright Copyright (C) 2018 DigitalGlobe (http://www.digitalglobe.com/)
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

typedef boost::shared_ptr<AddressTagKeys> AddressTagKeysPtr;

/**
 *
 */
class AddressTagKeys
{
public:

  AddressTagKeys();

  static const AddressTagKeysPtr& getInstance();

  /**
   *
   *
   * @param element
   * @return
   */
  QSet<QString> getAddressTagKeys(const Element& element) const;

  /**
   *
   *
   * @param tags
   * @param addressTagType
   * @return
   */
  QString getAddressTagKey(const Tags& tags, const QString addressTagType) const;

  /**
   * Returns an address tag value for an address type
   *
   * @param tags tags to search for address value
   * @param addressTagType address tag type as defined in the address tag keys config file
   * @return an address value
   */
  QString getAddressTagValue(const Tags& tags, const QString addressTagType) const;

  QSet<QString> getAdditionalTagKeys() const { return _additionalTagKeys; }

private:

  friend class PoiPolygonAddressScoreExtractorTest;

  static AddressTagKeysPtr _theInstance;

  //extra tags to search for addresses in
  QSet<QString> _additionalTagKeys;
  //maps address tag types to valid address tag keys
  QMultiMap<QString, QString> _addressTypeToTagKeys;

  /*
   * Reads tag keys used to identify tags as addresses
   */
  void _readAddressTagKeys(const QString configFile);
};

}

#endif // ADDRESS_TAG_KEYS_H
