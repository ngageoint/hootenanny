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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef ADDRESS_NORMALIZER_H
#define ADDRESS_NORMALIZER_H

// Hoot
#include <hoot/core/elements/Element.h>
#include <hoot/core/conflate/address/AddressTagKeys.h>

// Qt
#include <QSet>

namespace hoot
{

/**
 * @brief The AddressNormalizer class normalizes address tag values to a standard format using
 * libpostal.
 */
class AddressNormalizer
{
public:

  AddressNormalizer();

  /**
   * @brief normalizeAddresses normalizes the values of all address tags on an element.
   * @param e the element to normalize the addresses of
   */
  void normalizeAddresses(const ElementPtr& e) const;
  /**
   * @brief normalizeAddress normalizes address strings.
   *
   * It also translates addresses to English.
   * @param address the address to normalize
   * @return a collection of normalized addresses
   */
  QSet<QString> normalizeAddress(const QString& address) const;

  int getNumNormalized() const { return _numNormalized; }
  AddressTagKeysPtr getAddressTagKeys() const { return _addressTagKeys; }

private:

  mutable int _numNormalized;

  AddressTagKeysPtr _addressTagKeys;

  /*
   * libpostal has a few weird quirks, so we have to hack the input address a bit (hopefully goes
   * away at some point)
   */
  static void _prepareAddressForLibPostalNormalization(QString& address);

  QSet<QString> _normalizeAddressWithLibPostal(const QString& address) const;
  QSet<QString> _normalizeAddressIntersection(const QString& address) const;

  static bool _isValidNormalizedAddress(const QString& inputAddress,
                                        const QString& normalizedAddress);
};

}

#endif // ADDRESS_PARSER_H
