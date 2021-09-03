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

#ifndef HAS_PHONE_NUMBER_CRITERION_H
#define HAS_PHONE_NUMBER_CRITERION_H

#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/conflate/phone/PhoneNumberParser.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{

/**
 * Identifies features with phone numbers
 */
class HasPhoneNumberCriterion : public ElementCriterion, public Configurable
{
public:
  static QString className() { return "HasPhoneNumberCriterion"; }

  HasPhoneNumberCriterion() = default;
  ~HasPhoneNumberCriterion() = default;

  void setConfiguration(const Settings& conf) override;

  bool isSatisfied(const ConstElementPtr& e) const override;
  ElementCriterionPtr clone() override { return std::make_shared<HasPhoneNumberCriterion>(); }

  QString getDescription() const override { return "Identifies features with phone numbers"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString toString() const override { return className(); }

private:

  PhoneNumberParser _phoneNumberParser;
};

}
#endif // HAS_PHONE_NUMBER_CRITERION_H
