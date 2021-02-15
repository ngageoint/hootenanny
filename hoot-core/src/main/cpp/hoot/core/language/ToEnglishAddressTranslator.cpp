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
#include "ToEnglishAddressTranslator.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/StringUtils.h>

// libpostal
#include <libpostal/libpostal.h>


namespace hoot
{

std::shared_ptr<ToEnglishTranslator> ToEnglishAddressTranslator::_translator;

void ToEnglishAddressTranslator::setConfiguration(const Settings& conf)
{
  ConfigOptions config = ConfigOptions(conf);
  if (!_translator)
  {
    _translator.reset(
      Factory::getInstance().constructObject<ToEnglishTranslator>(
        config.getLanguageTranslationTranslator()));
    _translator->setConfiguration(conf);
    _translator->setSourceLanguages(config.getLanguageTranslationSourceLanguages());
    _translator->setId(className());
  }
}

QString ToEnglishAddressTranslator::translateToEnglish(const QString& address) const
{
  const QStringList addressParts = address.simplified().split(" ");
  //Try to translate blocks of consecutive address tokens to cut down on the number of
  //translation calls made.
  QString combinedAddressPartToTranslate = "";
  QStringList combinedAddressPartsToTranslate;
  for (int i = 0; i < addressParts.size(); i++)
  {
    const QString addressPart = addressParts.at(i);
    LOG_VART(addressPart);
    if (!StringUtils::isNumber(addressPart))
    {
      combinedAddressPartToTranslate += " " + addressPart;
      LOG_VART(combinedAddressPartToTranslate);
    }
    else if (!combinedAddressPartToTranslate.isEmpty())
    {
      combinedAddressPartsToTranslate.append(combinedAddressPartToTranslate.trimmed());
      combinedAddressPartToTranslate = "";
    }
    LOG_VART(combinedAddressPartsToTranslate);
  }
  if (!combinedAddressPartToTranslate.isEmpty())
  {
    combinedAddressPartsToTranslate.append(combinedAddressPartToTranslate.trimmed());
  }
  LOG_VART(combinedAddressPartsToTranslate);

  bool anyAddressPartWasTranslated = false;
  QString translatedAddress = address;
  for (int i = 0; i < combinedAddressPartsToTranslate.size(); i++)
  {
    const QString combinedAddressPart = combinedAddressPartsToTranslate.at(i).trimmed();
    LOG_VART(combinedAddressPart);
    const QString translatedCombinedAddressPart = _translator->translate(combinedAddressPart);
    if (!translatedCombinedAddressPart.isEmpty())
    {
      translatedAddress =
        translatedAddress.replace(combinedAddressPart, translatedCombinedAddressPart);
      LOG_VART(translatedAddress);
      anyAddressPartWasTranslated = true;
    }
  }

  if (anyAddressPartWasTranslated)
  {
    LOG_TRACE("Translated address from " << address << " to " << translatedAddress);
    return translatedAddress;
  }
  else
  {
    LOG_TRACE("Address " << address << " could not be translated.");
    return "";
  }
}

}
