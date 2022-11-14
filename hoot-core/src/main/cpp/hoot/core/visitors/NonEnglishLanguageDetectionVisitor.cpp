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

#include "NonEnglishLanguageDetectionVisitor.h"

// hoot
#include <hoot/core/language/HootServicesLanguageInfoResponseParser.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Settings.h>

#include <QTextStream>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, NonEnglishLanguageDetectionVisitor)

NonEnglishLanguageDetectionVisitor::NonEnglishLanguageDetectionVisitor()
  : _ignorePreTranslatedTags(false),
    _writeDetectedLangTags(false),
    _parseNames(false),
    _detectionSummary(""),
    _currentElementHasSuccessfulTagDetection(false),
    _numTagDetectionsMade(0),
    _numElementsWithSuccessfulTagDetection(0),
    _numTotalElements(0),
    _numProcessedTags(0),
    _numProcessedElements(0),
    _taskStatusUpdateInterval(10000)
{
}

NonEnglishLanguageDetectionVisitor::~NonEnglishLanguageDetectionVisitor()
{
  _printDetailedSummary();
}

QString NonEnglishLanguageDetectionVisitor::getInitStatusMessage() const
{
  _detectionSummary = "";
  return "Detecting source languages...";
}

QString NonEnglishLanguageDetectionVisitor::getCompletedStatusMessage() const
{
  //check the last element parsed to see if it had any successful tag translations
  if (_currentElementHasSuccessfulTagDetection)
  {
    _numElementsWithSuccessfulTagDetection++;
    _currentElementHasSuccessfulTagDetection = false;
  }
  _detectionSummary += QString("%1 successful tag detections made on %2 different elements.")
                        .arg(QString::number(_numTagDetectionsMade), QString::number(_numElementsWithSuccessfulTagDetection));
  return _detectionSummary;
}

void NonEnglishLanguageDetectionVisitor::_printDetailedSummary() const
{
  LOG_DEBUG("");
  LOG_DEBUG(getCompletedStatusMessage());
  LOG_DEBUG(
    _numTagDetectionsMade << " successful tag detections made on " <<
    _numElementsWithSuccessfulTagDetection << " different elements.");
  LOG_DEBUG(
    "Detected languages for " << _numTagDetectionsMade << " tags out of " << _numProcessedTags <<
    " encountered.");
  LOG_DEBUG(
    "Attempted to detect languages on tags for " << _numProcessedElements << " elements out of " <<
    _numTotalElements << " elements encountered.");
  const QString nameSortedCounts = _getLangCountsSortedByLangName();
  if (!nameSortedCounts.isEmpty())
  {
    LOG_DEBUG(nameSortedCounts);
  }
}

void NonEnglishLanguageDetectionVisitor::setConfiguration(const Settings& conf)
{
  ConfigOptions opts(conf);

  _infoClient = Factory::getInstance().constructObject<LanguageInfoProvider>(opts.getLanguageInfoProvider());
  _infoClient->setConfiguration(conf);
  _langCodesToLangs = HootServicesLanguageInfoResponseParser::getLangCodesToLangs(_infoClient->getAvailableLanguages("detectable"));
  LOG_VART(_langCodesToLangs.size());

  _langDetector = Factory::getInstance().constructObject<LanguageDetector>(opts.getLanguageDetectionDetector());
  _langDetector->setConfiguration(conf);

  _tagKeys = opts.getLanguageTagKeys().toSet();
  _ignorePreTranslatedTags = opts.getLanguageIgnorePreTranslatedTags();
  _taskStatusUpdateInterval = opts.getTaskStatusUpdateInterval();
  _writeDetectedLangTags = opts.getLanguageDetectionWriteDetectedLangTags();
  _parseNames = opts.getLanguageParseNames();
  if (_parseNames)
    _tagKeys = _tagKeys.unite(Tags::getNameKeys().toSet());
  LOG_VARD(_tagKeys);
}

QString NonEnglishLanguageDetectionVisitor::_getLangCountsSortedByLangName() const
{
  QString buffer;
  QTextStream ts(&buffer);
  ts.setCodec("UTF-8");
  ts << "Non-English language tag counts:";
  for (auto langsItr = _langNamesToCounts.begin(); langsItr != _langNamesToCounts.end(); ++langsItr)
    ts << "\n" << langsItr.key() << ": " << QString::number(langsItr.value());
  return ts.readAll();
}

void NonEnglishLanguageDetectionVisitor::visit(const std::shared_ptr<Element>& e)
{
  if (_tagKeys.isEmpty())
    throw HootException("No tag keys specified for language detection.");

  LOG_VART(e);
  // Why would the element ever be null here?
  if (!e)
    return;

  //if this var was set while parsing the previous element, increment the counter now
  if (_currentElementHasSuccessfulTagDetection)
    _numElementsWithSuccessfulTagDetection++;
  _currentElementHasSuccessfulTagDetection = false;

  const Tags& tags = e->getTags();
  bool elementProcessed = false;
  for (const auto& tagKey : qAsConst(_tagKeys))
  {
    if (tags.contains(tagKey))
    {  
      const QString preTranslatedTagKey = tagKey + ":en";
      if (!_ignorePreTranslatedTags && tags.contains(preTranslatedTagKey))
      {
        LOG_TRACE(
          "Skipping language detection for element with pre-translated tag: " <<
          preTranslatedTagKey << "=" << tags.get(preTranslatedTagKey));
      }
      else
      {
        elementProcessed = true;

        const QString detectedLangCode = _langDetector->detect(tags.get(tagKey));
        if (!detectedLangCode.isEmpty())
        {
          const QString langName = _langCodesToLangs[detectedLangCode];
          if (_writeDetectedLangTags)
            e->getTags().appendValue("hoot:detected:source:language:" + tagKey, langName);

          if (_langNamesToCounts.contains(langName))
            _langNamesToCounts[langName] = _langNamesToCounts[langName] + 1;
          else
            _langNamesToCounts[langName] = 1;

          _numTagDetectionsMade++;
          _currentElementHasSuccessfulTagDetection = true;
          if (_numTagDetectionsMade % _taskStatusUpdateInterval == 0)
          {
            PROGRESS_INFO(
              _numTagDetectionsMade << " of " << _numProcessedTags << " tag language detections " <<
               "made on  " << _numProcessedElements << " of " << _numTotalElements << " elements.");
          }
        }

        _numProcessedTags++;
        if (_numProcessedTags % _taskStatusUpdateInterval == 0)
        {
          PROGRESS_DEBUG("Processed " << _numProcessedTags << " tags.");
        }
      }
    }
  }

  if (elementProcessed)
    _numProcessedElements++;

  _numTotalElements++;
}

}
