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

#include "HootServicesLanguageDetectorMockClient.h"

// hoot
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(LanguageDetector, HootServicesLanguageDetectorMockClient)

HootServicesLanguageDetectorMockClient::HootServicesLanguageDetectorMockClient() :
HootServicesLanguageDetectorClient()
{
  _mockDetections[QString::fromUtf8("wie alt bist du")] = QString::fromUtf8("de");
  _mockDetections[QString::fromUtf8("wie heissen sie")] = QString::fromUtf8("de");
  _mockDetections[QString::fromUtf8("buenos noches")] = QString::fromUtf8("es");
  _mockDetections[QString::fromUtf8("buenos dias")] = QString::fromUtf8("es");
  _mockDetections[QString::fromUtf8("computer store")] = QString::fromUtf8("en");

  _useCookies = false;
}

QString HootServicesLanguageDetectorMockClient::detect(const QString& text)
{
  LOG_DEBUG("Detecting source language for text: " << text << "...");
  _numDetectionsAttempted++;
  if (!_mockDetections.contains(text.normalized(QString::NormalizationForm_D)))
  {
    LOG_TRACE("No mock translation available.");
    return "";
  }

  QString detectedLangCode = _mockDetections[text];

  if (detectedLangCode == "en")
  {
    LOG_TRACE("Source language for text: " << text << " detected as English.");
    _numEnglishTextsSkipped++;
    detectedLangCode = "";
  }

  if (!detectedLangCode.isEmpty())
  {
    _numDetectionsMade++;
    LOG_DEBUG("Detected source language: " << detectedLangCode << " for text: " << text);
  }

  return detectedLangCode;
}

}
