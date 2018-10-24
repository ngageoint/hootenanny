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

#include "NonEnglishLanguageDetectionVisitor.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/util/Log.h>
#include <hoot/rnd/language/HootServicesTranslationInfoResponseParser.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ConstElementVisitor, NonEnglishLanguageDetectionVisitor)

NonEnglishLanguageDetectionVisitor::NonEnglishLanguageDetectionVisitor() :
_ignorePreTranslatedTags(false),
_writeDetectedLangTags(false),
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
  //check the last element parsed to see if it had any successful tag translations
  if (_currentElementHasSuccessfulTagDetection)
  {
    _numElementsWithSuccessfulTagDetection++;
  }

  LOG_INFO(
    _numTagDetectionsMade << " successful tag detections made on " <<
    _numElementsWithSuccessfulTagDetection << " different elements.");
  LOG_INFO(
    "Detected languages for " << _numTagDetectionsMade << " tags out of " << _numProcessedTags <<
    " encountered.");
  LOG_INFO(
    "Attempted to detect languages on tags for " << _numProcessedElements << " elements out of " <<
    _numTotalElements << " elements encountered.");

  _printLangCounts();
}

void NonEnglishLanguageDetectionVisitor::setConfiguration(const Settings& conf)
{
  ConfigOptions opts(conf);

  _infoClient.reset(
    Factory::getInstance().constructObject<TranslationInfoProvider>(
      opts.getLanguageInfoProvider()));
  _infoClient->setConfiguration(conf);
  _langCodesToLangs =
    HootServicesTranslationInfoResponseParser::getLangCodesToLangs(
      _infoClient->getAvailableLanguages("detectable"));

  _langDetector.reset(
    Factory::getInstance().constructObject<LanguageDetector>(
      opts.getLanguageDetectionDetector()));
  _langDetector->setConfiguration(conf);

  _tagKeys = opts.getLanguageTagKeys();
  _ignorePreTranslatedTags = opts.getLanguageIgnorePreTranslatedTags();
  _taskStatusUpdateInterval = opts.getTaskStatusUpdateInterval();
  _writeDetectedLangTags = opts.getLanguageDetectionWriteDetectedLangTags();
}

void NonEnglishLanguageDetectionVisitor::_printLangCounts()
{
  QMultiMap<int, QString> langCountsSwapped;
  QMapIterator<QString, int> langCountsItr(_langCounts);
  while (langCountsItr.hasNext())
  {
    langCountsSwapped.insert(langCountsItr.value(), langCountsItr.key());
  }

  QString langsStr;
  QList<int> counts = langCountsSwapped.keys();
  for (QList<int>::const_iterator countsItr = counts.begin(); countsItr != counts.end();
       ++countsItr)
  {
    const int count = *countsItr;

    QStringList langNamesForCount;
    QMap<int, QString>::const_iterator langCodesForCountItr = langCountsSwapped.constFind(count);
    while (langCodesForCountItr != langCountsSwapped.end() && langCodesForCountItr.key() == count)
    {
      langNamesForCount.append(_langCodesToLangs[*langCodesForCountItr]);
    }
    langNamesForCount.sort();

    for (int i = 0; i < langNamesForCount.size(); i++)
    {
      langsStr += langNamesForCount.at(i) + ": " + QString::number(count) + "\n";
    }
  }
  langsStr.chop(1);
  LOG_INFO(langsStr);
}

void NonEnglishLanguageDetectionVisitor::visit(const boost::shared_ptr<Element>& e)
{
  LOG_VART(e);

  //if this var was set while parsing the previous element, increment the counter now
  if (_currentElementHasSuccessfulTagDetection)
  {
    _numElementsWithSuccessfulTagDetection++;
  }
  _currentElementHasSuccessfulTagDetection = false;

  const Tags& tags = e->getTags();
  bool elementProcessed = false;
  for (int i = 0; i < _tagKeys.size(); i++)
  {
    const QString tagKey = _tagKeys.at(i);
    if (tags.contains(tagKey))
    {  
      const QString preTranslatedTagKey = tagKey + ":en";
      if (_ignorePreTranslatedTags && tags.contains(preTranslatedTagKey))
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
          if (_writeDetectedLangTags)
          {
            e->getTags().appendValue(
              "hoot:detected:source:language:" + tagKey, _langCodesToLangs[detectedLangCode]);
          }

          _numTagDetectionsMade++;
          if (_numTagDetectionsMade % _taskStatusUpdateInterval == 0)
          {
            PROGRESS_DEBUG("Made " << _numTagDetectionsMade << " language detections.");
          }
          _currentElementHasSuccessfulTagDetection = true;
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
  {
    _numProcessedElements++;
    if (_numProcessedElements % _taskStatusUpdateInterval == 0)
    {
      PROGRESS_INFO("Attempted language detection for " << _numProcessedElements << " elements.");
    }
  }

  _numTotalElements++;
  if (_numTotalElements % _taskStatusUpdateInterval == 0)
  {
    PROGRESS_INFO("Visited " << _numTotalElements << " elements.");
  }
}

}
