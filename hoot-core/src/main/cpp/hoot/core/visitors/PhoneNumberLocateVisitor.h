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
 * @copyright Copyright (C) 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef PHONE_NUMBER_LOCATE_VISITOR_H
#define PHONE_NUMBER_LOCATE_VISITOR_H

// Hoot
#include <hoot/core/visitors/ElementVisitor.h>
#include <hoot/core/conflate/phone/PhoneNumberLocator.h>
#include <hoot/core/conflate/phone/PhoneNumberParser.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{

/**
 * Writes tags to an element indicating the detected location associated with its phone number tags
 * using libphonenumber.
 *
 * City level location is the most granular detection possible.
 */
class PhoneNumberLocateVisitor : public ElementVisitor, public Configurable
{
public:

  static QString className() { return "PhoneNumberLocateVisitor"; }

  PhoneNumberLocateVisitor();
  ~PhoneNumberLocateVisitor() override = default;

  /**
   * @see Configurable
   */
  void setConfiguration(const Settings& conf) override;

  /**
   * @see ElementVisitor
   */
  void visit(const ElementPtr& e) override;

  QString getInitStatusMessage() const override
  { return "Locating elements with phone numbers..."; }
  QString getCompletedStatusMessage() const override
  {
    return
      "Located " + QString::number(_totalPhoneNumbersLocated) + " phone numbers on " +
      QString::number(_numAffected) + " different elements";
  }

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString getDescription() const override
  { return "Determines admin level locations for elements based on phone numbers"; }

private:

  friend class PhoneNumberLocateVisitorTest;

  PhoneNumberParser _phoneNumberParser;
  PhoneNumberLocator _phoneNumberLocator;

  int _totalPhoneNumbersLocated;
};

}

#endif // PHONE_NUMBER_LOCATE_VISITOR_H
