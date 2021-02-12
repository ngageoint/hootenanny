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
 * @copyright Copyright (C) 2018, 2019 Maxar (http://www.maxar.com/)
 */

#include "PhoneNumberLocator.h"

// Hoot
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Log.h>

// libphonenumber
#include <phonenumbers/phonenumberutil.h>
using namespace i18n::phonenumbers;

#include <unicode/locid.h>

namespace hoot
{

PhoneNumberLocator::PhoneNumberLocator() :
_numLocated(0)
{
}

PhoneNumberLocator::PhoneNumberLocator(const QString& regionCode) :
_regionCode(regionCode),
_numLocated(0)
{
}

void PhoneNumberLocator::setConfiguration(const Settings& conf)
{
  ConfigOptions config = ConfigOptions(conf);
  setRegionCode(config.getPhoneNumberRegionCode());
}

void PhoneNumberLocator::setRegionCode(const QString& code)
{
  _regionCode = code.trimmed().toUpper();
  if (!_regionCode.isEmpty())
  {
    std::set<std::string> regions;
      PhoneNumberUtil::GetInstance()->GetSupportedRegions(&regions);
    std::set<std::string>::const_iterator it = regions.find(_regionCode.toStdString());
    if (it == regions.end())
    {
      throw HootException("Invalid phone number region code: " + _regionCode);
    }
  }
}

QString PhoneNumberLocator::getLocationDescription(const QString& phoneNumber) const
{
  LOG_VART(phoneNumber);

  PhoneNumber parsedPhoneNumber;
  PhoneNumberUtil::ErrorType error =
    PhoneNumberUtil::GetInstance()->Parse(
      phoneNumber.toStdString(), _regionCode.toStdString(), &parsedPhoneNumber);
  LOG_VART(error);
  if (error == PhoneNumberUtil::ErrorType::NO_PARSING_ERROR)
  {
    const icu::Locale locale = icu::Locale("en", "US");
    QString locationDescription;
    if (!_regionCode.isEmpty())
    {
      locationDescription =
        QString::fromStdString(
          _geocoder.GetDescriptionForNumber(parsedPhoneNumber, locale, _regionCode.toStdString()));
    }
    else
    {
      locationDescription =
        QString::fromStdString(_geocoder.GetDescriptionForNumber(parsedPhoneNumber, locale));
    }
    _numLocated++;
    LOG_VART(locationDescription);
    return locationDescription;
  }
  LOG_TRACE("No location found for phone number: " << phoneNumber);
  return "";
}

}
