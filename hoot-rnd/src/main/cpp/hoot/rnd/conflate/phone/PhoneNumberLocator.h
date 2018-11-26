
#ifndef PHONE_NUMBER_LOCATOR_H
#define PHONE_NUMBER_LOCATOR_H

// Hoot
#include <hoot/core/elements/Element.h>

// Qt
#include <QString>

// libphonenumber
#include <phonenumbers/geocoding/phonenumber_offline_geocoder.h>
using namespace i18n::phonenumbers;

namespace hoot
{

/**
 *
 */
class PhoneNumberLocator
{
public:

  PhoneNumberLocator();
  PhoneNumberLocator(const QString regionCode);

  /**
   *
   *
   * @param phoneNumber
   * @return
   */
  QString getLocationDescription(const QString phoneNumber) const;

private:

  //required to validate phone numbers
  QString _regionCode;
  PhoneNumberOfflineGeocoder _geocoder;
};

}

#endif // PHONE_NUMBER_LOCATOR_H
