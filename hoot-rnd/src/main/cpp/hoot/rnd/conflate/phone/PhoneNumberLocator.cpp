
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

PhoneNumberLocator::PhoneNumberLocator()
{
}

PhoneNumberLocator::PhoneNumberLocator(const QString regionCode) :
_regionCode(regionCode)
{
}

QString PhoneNumberLocator::getLocationDescription(const QString phoneNumber) const
{
  PhoneNumber parsedPhoneNumber;
  PhoneNumberUtil::ErrorType error =
    PhoneNumberUtil::GetInstance()->Parse(
      phoneNumber.toStdString(), _regionCode.toStdString(), &parsedPhoneNumber);
  if (error == PhoneNumberUtil::ErrorType::NO_PARSING_ERROR)
  {
    const icu::Locale locale = icu::Locale("en", "US");
    if (!_regionCode.isEmpty())
    {
      return
        QString::fromStdString(
          _geocoder.GetDescriptionForNumber(parsedPhoneNumber, locale, _regionCode.toStdString()));
    }
    else
    {
      return QString::fromStdString(_geocoder.GetDescriptionForNumber(parsedPhoneNumber, locale));
    }
  }
  return "";
}

}
