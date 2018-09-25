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
  else if (_addressesMatchesOnSubLetter(_address, address._address) ||
           _addressesMatchesOnSubLetter(_address, address._translatedAddress))
  {
    areEqual = true;
  }
  else if (_addressesMatchesOnSubLetter(_translatedAddress, address._translatedAddress) ||
           _addressesMatchesOnSubLetter(_translatedAddress, address._address))
  {
    areEqual = true;
  }
  LOG_VART(areEqual);
  return areEqual;
}

bool PoiPolygonAddress::_addressesMatchesOnSubLetter(const QString polyAddress,
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
  LOG_VART(polyHouseNumStr);
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

    if (_addrComp.compare(subletterCleanedPolyAddress, subletterCleanedPoiAddress) == 1.0)
    {
      return true;
    }
  }

  return false;
}

}
