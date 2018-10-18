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
#include "PoiPolygonAddress.h"

// hoot
#include <hoot/core/util/Log.h>
#include "PoiPolygonAddressScoreExtractor.h"


namespace hoot
{

PoiPolygonAddress::PoiPolygonAddress() :
_address(""),
_translatedAddress("")
{
}

PoiPolygonAddress::PoiPolygonAddress(const QString address, const QString translatedAddress) :
_address(address),
_translatedAddress(translatedAddress)
{
}

bool PoiPolygonAddress::operator==(const PoiPolygonAddress& address) const
{
  LOG_VART(_address);
  LOG_VART(address._address);
  LOG_VART(_translatedAddress);
  LOG_VART(address._translatedAddress);

  bool areEqual = false;
  if (!_address.isEmpty() &&
      (_addrComp.compare(_address, address._address) == 1.0 ||
       _addrComp.compare(_address, address._translatedAddress) == 1.0))
  {
    areEqual = true;
  }
  else if (!_translatedAddress.isEmpty() &&
           (_addrComp.compare(_translatedAddress, address._translatedAddress) == 1.0 ||
            _addrComp.compare(_translatedAddress, address._address) == 1.0))
  {
    areEqual = true;
  }
  else if (PoiPolygonAddressScoreExtractor::addressesMatchDespiteSubletterDiffs(
             _address, address._address) ||
           PoiPolygonAddressScoreExtractor::addressesMatchDespiteSubletterDiffs(
             _address, address._translatedAddress))
  {
    areEqual = true;
  }
  else if (PoiPolygonAddressScoreExtractor::addressesMatchDespiteSubletterDiffs(
             _translatedAddress, address._translatedAddress) ||
           PoiPolygonAddressScoreExtractor::addressesMatchDespiteSubletterDiffs(
             _translatedAddress, address._address))
  {
    areEqual = true;
  }
  LOG_VART(areEqual);
  return areEqual;
}

}
