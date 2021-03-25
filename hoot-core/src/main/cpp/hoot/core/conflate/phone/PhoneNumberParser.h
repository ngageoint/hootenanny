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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef PHONE_NUMBER_PARSER_H
#define PHONE_NUMBER_PARSER_H

// Hoot
#include <hoot/core/util/Configurable.h>
#include <hoot/core/elements/Element.h>

// Qt
#include <QString>

namespace hoot
{

struct ElementPhoneNumber
{
  QString name;
  QString tagKey;
  QString tagValue;
};

/**
 * Parses phone numbers from features and validates them.
 *
 * Looks at tag keys containing "phone" by default and can be expanded with additional tag keys.
 */
class PhoneNumberParser : public Configurable
{
public:

  PhoneNumberParser();
  ~PhoneNumberParser() = default;

  void setConfiguration(const Settings& conf) override;

  /**
   * Parses phone numbers from an element's tags
   *
   * @param element the element to parse phone numbers from
   * @return a collection of phone numbers
   */
  QList<ElementPhoneNumber> parsePhoneNumbers(const Element& e) const;

  /**
   * Returns the number of valid phone number tags contained by the element
   *
   * @param element the element to examine for phone numbers
   * @return a phone number count
   */
  int numPhoneNumbers(const Element& e) const;

  /**
   * Determines if an element contains a phone number
   *
   * @param element the element to examine for a phone number
   * @return true if the element contains at least one phone number; false otherwise
   * @note This could be made more efficient, if necessary, by only counting the first phone number.
   */
  bool hasPhoneNumber(const Element& e) const;

  /**
   * Returns the number of valid phone number tags contained by the element
   *
   * @param element the element to examine for phone numbers
   * @return a phone number count
   */
  int numPhoneNumbers(const ConstElementPtr& element) const;

  /**
   * Determines if an element contains a phone number
   *
   * @param element the element to examine for a phone number
   * @return true if the element contains at least one phone number; false otherwise
   * @note This could be made more efficient, if necessary, by only counting the first phone number.
   */
  bool hasPhoneNumber(const ConstElementPtr& element) const;

  void setRegionCode(const QString& code);
  void setAdditionalTagKeys(const QStringList& keys) { _additionalTagKeys = keys; }
  void setSearchInText(bool search);
  long getPhoneNumbersProcessed() const { return _phoneNumbersProcessed; }

private:

  //required to validate phone numbers
  QString _regionCode;
  //user customizable fields to search
  QStringList _additionalTagKeys;
  //allows the parser to search through text tokens; otherwise, it tries to match entire strings
  //as phone numbers
  bool _searchInText;
  mutable long _phoneNumbersProcessed;

  void _addPhoneNumber(const QString& name, const QString& tagKey, const QString& tagValue,
                       QList<ElementPhoneNumber>& phoneNumbers) const;
};

}

#endif // PHONE_NUMBER_PARSER_H
