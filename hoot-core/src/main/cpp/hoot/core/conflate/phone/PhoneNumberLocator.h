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
 * @copyright Copyright (C) 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef PHONE_NUMBER_LOCATOR_H
#define PHONE_NUMBER_LOCATOR_H

// Hoot
#include <hoot/core/elements/Element.h>
#include <hoot/core/util/Configurable.h>

// Qt
#include <QString>

// libphonenumber
#include <phonenumbers/geocoding/phonenumber_offline_geocoder.h>

namespace hoot
{

/**
 * Attempts to determine the location of a phone number down to the city level using libphonenumber
 */
class PhoneNumberLocator : public Configurable
{
public:

  PhoneNumberLocator();
  PhoneNumberLocator(const QString& regionCode);
  virtual ~PhoneNumberLocator() = default;

  virtual void setConfiguration(const Settings& conf);

  /**
   * Returns an admin level address string for a given phone number
   *
   * @param phoneNumber the phone number to locate
   * @return an address string
   */
  QString getLocationDescription(const QString& phoneNumber) const;

  void setRegionCode(const QString& code);

  int getNumLocated() const { return _numLocated; }

private:

  //two digit location code required to validate phone numbers; see ConfigOptions
  QString _regionCode;
  i18n::phonenumbers::PhoneNumberOfflineGeocoder _geocoder;
  mutable int _numLocated;
};

}

#endif // PHONE_NUMBER_LOCATOR_H
