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
 * @copyright Copyright (C) 2015-2023 Maxar (http://www.maxar.com/)
 */

#include "PhoneNumberParser.h"

// Hoot
#include <hoot/core/elements/Element.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/StringUtils.h>

// libphonenumber
#include <phonenumbers/phonenumberutil.h>
#include <phonenumbers/phonenumbermatcher.h>
#include <phonenumbers/phonenumbermatch.h>
using namespace i18n::phonenumbers;

namespace hoot
{

PhoneNumberParser::PhoneNumberParser()
  : _regionCode(""),
    _searchInText(false),
    _phoneNumbersProcessed(0)
{
}

void PhoneNumberParser::setSearchInText(bool search)
{
  if (search && _regionCode.isEmpty())
    throw HootException("A region code must be set when searching for phone numbers in text.");
  _searchInText = search;
}

void PhoneNumberParser::setRegionCode(const QString& code)
{
  _regionCode = code.trimmed().toUpper();
  if (!_regionCode.isEmpty())
  {
    std::set<std::string> regions;
    PhoneNumberUtil::GetInstance()->GetSupportedRegions(&regions);
    if (regions.find(_regionCode.toStdString()) == regions.end())
      throw HootException("Invalid phone number region code: " + _regionCode);
  }
}

void PhoneNumberParser::setConfiguration(const Settings& conf)
{
  ConfigOptions config = ConfigOptions(conf);
  setRegionCode(config.getPhoneNumberRegionCode());
  setAdditionalTagKeys(config.getPhoneNumberAdditionalTagKeys());
  setSearchInText(config.getPhoneNumberSearchInText());
}

void PhoneNumberParser::_addPhoneNumber(const QString& name, const QString& tagKey,
                                        const QString& tagValue,
                                        QList<ElementPhoneNumber>& phoneNumbers) const
{
  // Not normalizing by default here, since it will happen during phone number matching.
  LOG_TRACE("Possibly a phone number: " << tagKey << "=" << tagValue);
  ElementPhoneNumber elementPhoneNumber;
  elementPhoneNumber.name = name;
  elementPhoneNumber.tagKey = tagKey;
  elementPhoneNumber.tagValue = tagValue;
  phoneNumbers.append(elementPhoneNumber);
  _phoneNumbersProcessed++;
}

int PhoneNumberParser::numPhoneNumbers(const Element& element) const
{
  return parsePhoneNumbers(element).size();
}

bool PhoneNumberParser::hasPhoneNumber(const Element& element) const
{
  return numPhoneNumbers(element) > 0;
}

QList<ElementPhoneNumber> PhoneNumberParser::parsePhoneNumbers(const Element& element) const
{
  //phone=* is the standard OSM tag, but have seen many others over time...keeping the allowed tags
  //fairly loose for now
  QList<ElementPhoneNumber> parsedPhoneNums;
  const Tags& tags = element.getTags();
  for (auto it = tags.constBegin(); it != tags.constEnd(); ++it)
  {
    const QString tagKey = it.key();
    LOG_VART(tagKey);
    if (_additionalTagKeys.contains(tagKey, Qt::CaseInsensitive) || tagKey.contains("phone", Qt::CaseInsensitive))
    {
      const QString tagValue = it.value().toUtf8().trimmed().simplified();
      LOG_VART(tagValue);

      if (_regionCode.isEmpty())
      {
        // If we're not using a region code to see if the number is valid, then just check for at
        // least one digit (vanity numbers can have letters).
        // consider getting rid of this, as its too weak of a check
        if (StringUtils::hasDigit(tagValue))
          _addPhoneNumber(element.getTags().getName(), tagKey, tagValue, parsedPhoneNums);
      }
      else
      {
        if (!_searchInText)
        {
          // IsPossibleNumber is a fairly quick check (IsValidNumber is more strict and a little
          // more expensive)
          if (PhoneNumberUtil::GetInstance()->IsPossibleNumberForString(tagValue.toStdString(), _regionCode.toStdString()))
            _addPhoneNumber(element.getTags().getName(), tagKey, tagValue, parsedPhoneNums);
        }
        else
        {
          // This lets us search through text that may have other things in it besides phone
          // numbers.
          PhoneNumberMatcher numberFinder(
            *PhoneNumberUtil::GetInstance(),
            tagValue.toStdString(),
            _regionCode.toStdString(),
            PhoneNumberMatcher::Leniency::POSSIBLE,
            //not sure what a good number is for max tries yet or what that number actually even
            //means
            1);
          int parserFinds = 0;
          while (numberFinder.HasNext())
          {
            PhoneNumberMatch match;
            numberFinder.Next(&match);
            const QString parsedNum = QString::fromStdString(match.raw_string());
            _addPhoneNumber(element.getTags().getName(), tagKey, parsedNum, parsedPhoneNums);
            parserFinds++;
          }
          LOG_TRACE("Number finder found " << parserFinds << " numbers.");
        }
      }

      if (parsedPhoneNums.empty())
      {
        LOG_TRACE("Not a phone number: " << tagKey << "=" << tagValue);
      }
    }
  }
  return parsedPhoneNums;
}

}
