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
 * @copyright Copyright (C) 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#ifndef NORMALIZE_PHONE_NUMBERS_VISITOR_H
#define NORMALIZE_PHONE_NUMBERS_VISITOR_H

// hoot
#include <hoot/core/conflate/phone/PhoneNumberNormalizer.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/visitors/ElementVisitor.h>

namespace hoot
{

/**
 * Normalizes element phone numbers
 */
class NormalizePhoneNumbersVisitor : public ElementVisitor, public Configurable
{
public:

  static QString className() { return "NormalizePhoneNumbersVisitor"; }

  NormalizePhoneNumbersVisitor() = default;
  ~NormalizePhoneNumbersVisitor() override = default;

  /**
   * @see Configurable
   */
  void setConfiguration(const Settings& conf) override;

  /**
   * @see ElementVisitor
   */
  void visit(const ElementPtr& e) override;

  QString getInitStatusMessage() const override { return "Normalizing phone numbers..."; }
  QString getCompletedStatusMessage() const override
  {
    return QString("Normalized %1 phone numbers").arg(QString::number(_phoneNumberNormalizer.getNumNormalized()));
  }

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString getDescription() const override { return "Normalizes phone numbers"; }

private:

  friend class NormalizePhoneNumbersVisitorTest;

  PhoneNumberNormalizer _phoneNumberNormalizer;
};

}

#endif // NORMALIZE_PHONE_NUMBERS_VISITOR_H
