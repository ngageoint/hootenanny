
#ifndef PHONE_NUMBER_LOCATOR_H
#define PHONE_NUMBER_LOCATOR_H

// Hoot
#include <hoot/core/elements/Element.h>
#include <hoot/core/util/Configurable.h>

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
class PhoneNumberLocator : public Configurable
{
public:

  PhoneNumberLocator();
  PhoneNumberLocator(const QString regionCode);

  virtual void setConfiguration(const Settings& conf);

  /**
   *
   *
   * @param phoneNumber
   * @return
   */
  QString getLocationDescription(const QString phoneNumber) const;

  void setRegionCode(QString code);

  int getNumLocated() const { return _numLocated; }

private:

  //required to validate phone numbers
  QString _regionCode;
  PhoneNumberOfflineGeocoder _geocoder;
  mutable int _numLocated;
};

}

#endif // PHONE_NUMBER_LOCATOR_H
