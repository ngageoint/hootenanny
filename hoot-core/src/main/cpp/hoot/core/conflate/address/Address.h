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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef ADDRESSS_H
#define ADDRESSS_H

// hoot
#include <hoot/core/algorithms/string/ExactStringDistance.h>

// Qt
#include <QSet>
#include <QMap>

namespace hoot
{

/**
 * Encapsulates a street address for conflation purposes
 */
class Address
{

public:

  Address();
  Address(const QString& address, const bool allowLenientHouseNumberMatching = true);

  bool operator==(const Address& address) const;

  QString toString() const { return "Address: " + _address; }

  /**
   * TODO
   *
   * @param addressStr
   * @param requireStreetTypeInIntersection
   * @return
   */
  static bool isIntersectionAddress(const QString& addressStr,
                                    const bool requireStreetTypeInIntersection = false);

  /**
   * TODO
   *
   * @param address
   * @param requireStreetTypeInIntersection
   * @return
   */
  static bool isIntersectionAddress(const Address& address,
                                    const bool requireStreetTypeInIntersection = false);

  /**
   * TODO
   *
   * @param includeAbbreviations
   * @return
   */
  static QSet<QString> getStreetTypes(const bool includeAbbreviations = true);

  /**
   * TODO
   *
   * @return
   */
  static QMap<QString, QString> getStreetFullTypesToTypeAbbreviations();

  /**
   * TODO
   *
   * @return
   */
  static QMap<QString, QString> getStreetTypeAbbreviationsToFullTypes();

  /**
   * TODO
   *
   * @return
   */
  static QStringList getIntersectionSplitTokens();

  /**
   * TODO
   */
  void removeStreetTypes();

private:

  QString _address;
  ExactStringDistance _addrComp;
  //see AddressParser::addressesMatchDespiteSubletterDiffs
  bool _allowLenientHouseNumberMatching;

  // TODO
  static QSet<QString> _streetTypes;
  // TODO
  static QMap<QString, QString> _streetFullTypesToTypeAbbreviations;
  // TODO
  static QMap<QString, QString> _streetTypeAbbreviationsToFullTypes;
};

}

#endif // POI_POLYGON_ADDRESSS_H
