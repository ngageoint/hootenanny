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
 * @copyright Copyright (C) 2015, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef PHONE_NUMBER_NORMALIZER_H
#define PHONE_NUMBER_NORMALIZER_H

// Hoot
#include <hoot/core/util/Configurable.h>
#include <hoot/core/elements/Element.h>

// Qt
#include <QString>

namespace hoot
{

/**
 * Parses phone numbers from features and normalizes them.
 *
 * See PhoneNumberParser for related notes.
 */
class PhoneNumberNormalizer : public Configurable
{
public:

  PhoneNumberNormalizer();

  virtual void setConfiguration(const Settings& conf);

  /**
   *
   *
   * @param element
   * @return
   */
  void normalizePhoneNumbers(const ElementPtr& element);

  void setRegionCode(QString code);
  void setAdditionalTagKeys(QStringList keys) { _additionalTagKeys = keys; }
  void setSearchInText(bool search);

private:

  //required to validate phone numbers
  QString _regionCode;
  //user customizable fields to search
  QStringList _additionalTagKeys;
  //allows the parser to search through text tokens; otherwise, it tries to match entire strings
  //as phone numbers
  bool _searchInText;
};

}

#endif // PHONE_NUMBER_NORMALIZER_H
