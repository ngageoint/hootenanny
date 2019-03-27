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

#include "PhoneNumberLocateVisitor.h"

#include <hoot/core/util/ConfigOptions.h>

namespace hoot
{

PhoneNumberLocateVisitor::PhoneNumberLocateVisitor()
{

}

void PhoneNumberLocateVisitor::setConfiguration(const Settings& conf)
{
  _phoneNumberLocator.setConfiguration(conf);
}

void PhoneNumberLocateVisitor::visit(const ElementPtr& e)
{
  QList<ElementPhoneNumber> phoneNumbers = _phoneNumberParser.parsePhoneNumbers(*e);
  bool anyPhoneNumberLocated = false;
  for (QList<ElementPhoneNumber>::const_iterator phoneNumberItr = phoneNumbers.constBegin();
       phoneNumberItr != phoneNumbers.constEnd(); ++phoneNumberItr)
  {
    ElementPhoneNumber phoneNumber = *phoneNumberItr;
    const QString locationDescription =
      _phoneNumberLocator.getLocationDescription(phoneNumber.tagValue);
    if (!locationDescription.isEmpty())
    {
      e->getTags().set(phoneNumber.tagKey + ":location", locationDescription);
      _totalPhoneNumbersLocated++;
      anyPhoneNumberLocated = true;
    }
  }

  if (anyPhoneNumberLocated)
  {
    _numAffected++;
  }
}

}
