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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef POIPOLYGONADDRESSMATCH_H
#define POIPOLYGONADDRESSMATCH_H

// hoot
#include <hoot/core/OsmMap.h>

namespace hoot
{

/**
 * Determines if two features have an address match
 */
class PoiPolygonAddressMatch
{
public:

  static const QChar ESZETT;
  static const QString ESZETT_REPLACE;
  static const QString HOUSE_NUMBER_TAG_NAME;
  static const QString STREET_TAG_NAME;
  static const QString FULL_ADDRESS_TAG_NAME;
  static const QString FULL_ADDRESS_TAG_NAME_2;

  PoiPolygonAddressMatch(const ConstOsmMapPtr& map);

  bool isMatch(ConstElementPtr poly, ConstElementPtr poi);

private:

  const ConstOsmMapPtr _map;

  void _collectAddressesFromElement(ConstElementPtr element,  QStringList& addresses);
  void _collectAddressesFromWay(ConstWayPtr way, QStringList& addresses);
  void _collectAddressesFromRelation(ConstRelationPtr relation, QStringList& addresses);
  void _parseAddressesAsRange(const QString houseNum, const QString street, QStringList& addresses);
  void _parseAddressesInAltFormat(const Tags& tags, QStringList& addresses);
  bool _addressesMatchesOnSubLetter(const QString polyAddress, const QString poiAddress);

};

}

#endif // POIPOLYGONADDRESSMATCH_H
