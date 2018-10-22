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
#ifndef ADDRESS_PARSER_H
#define ADDRESS_PARSER_H

// hoot
#include <hoot/core/util/Configurable.h>

// Qt
#include <QMultiMap>
#include <QSet>

namespace hoot
{

class Element;
class Tags;

/**
 * TODO
 */
class AddressParser : public Configurable
{
public:

  AddressParser();

  virtual void setConfiguration(const Settings& conf);

  /**
   * TODO
   *
   * @param element
   * @param houseNum
   * @param street
   * @return
   */
  QSet<QString> parseAddresses(const Element& element, QString& houseNum, QString& street) const;

  /**
   * TODO
   *
   * normalize also translates to English
   *
   * @param address
   * @return
   */
  QSet<QString> normalizeAddress(const QString address) const;

  /**
   * Returns an address tag value for an address type
   *
   * @param tags tags to search for address value
   * @param addressTagType address tag type as defined in the address tag keys config file
   * @return an address value
   */
  static QString getAddressTagValue(const Tags& tags, const QString addressTagType);

  /**
   * Determines whether two addresses match despite house number subletter differences
   *
   * So, allow "34 elm street" to match "34a elm street".
   *
   * @param address1 first address to compare
   * @param address2 second address to compare
   * @return true if the address match despite subletter differences; false otherwise
   */
  static bool addressesMatchDespiteSubletterDiffs(const QString address1, const QString address2);

  void setAdditionalTagKeys(QSet<QString> keys) { _additionalTagKeys = keys; }

private:

  QSet<QString> _additionalTagKeys;
  static QMultiMap<QString, QString> _addressTypeToTagKeys;

  static void _readAddressTagKeys(const QString configFile);

  QSet<QString> _parseAddressAsRange(const QString houseNum, const QString street) const;
  bool _isRangeAddress(const QString houseNum) const;
  bool _isParseableAddressFromComponents(const Tags& tags, QString& houseNum,
                                         QString& street) const;
  bool _isValidAddressStr(QString& address, QString& houseNum,  QString& street) const;
  QString _parseFullAddress(const QString fullAddress, QString& houseNum, QString& street) const;
  QSet<QString> _parseAddressFromComponents(const Tags& tags, QString& houseNum,
                                            QString& street) const;
  QString _parseAddressFromAltTags(const Tags& tags, QString& houseNum, QString& street) const;
  static QString _getSubLetterCleanedAddress(const QString address);
};

}

#endif // ADDRESS_PARSER_H
