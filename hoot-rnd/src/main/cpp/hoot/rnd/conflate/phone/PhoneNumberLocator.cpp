
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

PhoneNumberLocator::PhoneNumberLocator(const QString regionCode) :
_regionCode(regionCode),
_numLocated(0)
{
}

void PhoneNumberLocator::setConfiguration(const Settings& conf)
{
  ConfigOptions config = ConfigOptions(conf);
  setRegionCode(config.getPhoneNumberRegionCode());
}

void PhoneNumberLocator::setRegionCode(QString code)
{
  code = code.trimmed().toUpper();
  if (!code.isEmpty())
  {
    std::set<std::string> regions;
      PhoneNumberUtil::GetInstance()->GetSupportedRegions(&regions);
    std::set<std::string>::const_iterator it = regions.find(code.toStdString());
    if (it == regions.end())
    {
      throw HootException("Invalid phone number region code: " + code);
    }
  }
  _regionCode = code;
}

QString PhoneNumberLocator::getLocationDescription(const QString phoneNumber) const
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
