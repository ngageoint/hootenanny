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
 * @copyright Copyright (C) 2015, 2017, 2018, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef PHONE_NUMBER_NORMALIZER_H
#define PHONE_NUMBER_NORMALIZER_H

// Hoot
#include <hoot/core/util/Configurable.h>
#include <hoot/core/elements/Element.h>

// Qt
#include <QString>

// libphonenumber
#include <phonenumbers/phonenumberutil.h>

namespace hoot
{

/**
 * Normalizes phone numbers
 *
 * See PhoneNumberParser for related notes.
 */
class PhoneNumberNormalizer : public Configurable
{
public:

  PhoneNumberNormalizer();
  virtual ~PhoneNumberNormalizer() = default;

  /**
   * @see Configurable
   */
  virtual void setConfiguration(const Settings& conf);

  /**
   * Normalizes the values of all address tags on an element
   *
   * @param element the element to normalize the phone numbers of
   */
  void normalizePhoneNumbers(const ElementPtr& element);

  void setRegionCode(QString code);
  void setAdditionalTagKeys(QStringList keys) { _additionalTagKeys = keys; }
  void setSearchInText(bool search);
  void setFormat(QString format);

  int getNumNormalized() const { return _numNormalized; }

private:

  //two digit location code required to validate phone numbers; see ConfigOptions
  QString _regionCode;
  //user customizable fields to search in addition to the default fields
  QStringList _additionalTagKeys;
  //allows the parser to search through text tokens; otherwise, it tries to match entire strings
  //as phone numbers
  bool _searchInText;
  //format to use during normalization; see ConfigOptions
  i18n::phonenumbers::PhoneNumberUtil::PhoneNumberFormat _format;
  int _numNormalized;
};

}

#endif // PHONE_NUMBER_NORMALIZER_H
