
#include "PhoneNumberLocator.h"

// Hoot
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Log.h>

namespace hoot
{

PhoneNumberLocator::PhoneNumberLocator() :
_regionCode("")
{
}

QString PhoneNumberLocator::locate(const QString phoneNumber) const
{
  //TODO:
  //_geocoder
  return phoneNumber;
}

}
