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
 * @copyright Copyright (C) 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "HootServicesTranslatorMockClient.h"

// hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ToEnglishTranslator, HootServicesTranslatorMockClient)

HootServicesTranslatorMockClient::HootServicesTranslatorMockClient() :
HootServicesTranslatorClient()
{
  _mockTranslations["wie alt bist du"] = "how old are you;German";
  _mockTranslations["buenos noches"] = "good night;Spanish";
}

void HootServicesTranslatorMockClient::translate(const QString textToTranslate)
{
  if (_sourceLangs.size() == 0)
  {
    throw HootException("Cannot determine source language.");
  }
  if (!_mockTranslations.contains(textToTranslate))
  {
    throw HootException("No mock translation available.");
  }

  LOG_DEBUG(
    "Translating to English with specified source languages: " <<
     _sourceLangs.join(",") << "; text: " << textToTranslate);

  _translatedText = _mockTranslations[textToTranslate].split(";")[0];
  if (_sourceLangs.size() > 1)
  {
    _detectedLang = _mockTranslations[textToTranslate].split(";")[1];
    _detectionMade = true;
    _detectorUsed = "TikaLanguageDetector";
    _detectedLangAvailableForTranslation = true;
  }

  emit translationComplete();
}

}
