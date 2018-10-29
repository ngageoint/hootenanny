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
#include <hoot/core/language/HootServicesLanguageInfoResponseParser.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, NonEnglishLanguageDetectionVisitor)

NonEnglishLanguageDetectionVisitor::NonEnglishLanguageDetectionVisitor() :
_ignorePreTranslatedTags(false),
_writeDetectedLangTags(false),
_parseNames(false),
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

  LOG_INFO("");
  LOG_INFO(
    _numTagDetectionsMade << " successful tag detections made on " <<
    _numElementsWithSuccessfulTagDetection << " different elements.");
  LOG_INFO(
    "Detected languages for " << _numTagDetectionsMade << " tags out of " << _numProcessedTags <<
    " encountered.");
  LOG_INFO(
    "Attempted to detect languages on tags for " << _numProcessedElements << " elements out of " <<
    _numTotalElements << " elements encountered.");
  const QString freqSortedCounts = getLangCountsSortedByFrequency();
  if (!freqSortedCounts.isEmpty())
  {
    LOG_INFO(freqSortedCounts);
  }
  const QString nameSortedCounts = getLangCountsSortedByLangName();
  if (!nameSortedCounts.isEmpty())
  {
    LOG_INFO(nameSortedCounts);
  }
}

void NonEnglishLanguageDetectionVisitor::setConfiguration(const Settings& conf)
{
  ConfigOptions opts(conf);

  _infoClient.reset(
    Factory::getInstance().constructObject<LanguageInfoProvider>(
      opts.getLanguageInfoProvider()));
  _infoClient->setConfiguration(conf);
  _langCodesToLangs =
    HootServicesLanguageInfoResponseParser::getLangCodesToLangs(
      _infoClient->getAvailableLanguages("detectable"));
  LOG_VART(_langCodesToLangs.size());

  _langDetector.reset(
    Factory::getInstance().constructObject<LanguageDetector>(
      opts.getLanguageDetectionDetector()));
  _langDetector->setConfiguration(conf);

  _tagKeys = opts.getLanguageTagKeys().toSet();
  _ignorePreTranslatedTags = opts.getLanguageIgnorePreTranslatedTags();
  _taskStatusUpdateInterval = opts.getTaskStatusUpdateInterval();
  _writeDetectedLangTags = opts.getLanguageDetectionWriteDetectedLangTags();
  _parseNames = opts.getLanguageParseNames();
  if (_parseNames)
  {
    _tagKeys = _tagKeys.unite(Tags::getNameKeys().toSet());
  }
  LOG_VARD(_tagKeys);
}

QString NonEnglishLanguageDetectionVisitor::getLangCountsSortedByFrequency() const
{
  QString langsStr;

  LOG_VART(_langNamesToCounts.size());
  QMultiMap<int, QString> langCountsSwapped;
  for (QMap<QString, int>::const_iterator langCountsItr = _langNamesToCounts.begin();
       langCountsItr != _langNamesToCounts.end(); ++langCountsItr)
  {
    LOG_VART(langCountsItr.key());
    assert(langCountsItr.key() != "en");
    LOG_VART(langCountsItr.value());
    langCountsSwapped.insert(langCountsItr.value(), langCountsItr.key());
  }
  LOG_VART(langCountsSwapped.size());

  std::list<int> countsStl = langCountsSwapped.keys().toStdList();
  LOG_VART(countsStl.size());
  std::reverse(countsStl.begin(), countsStl.end());
  QList<int> counts = QList<int>::fromStdList(countsStl);
  LOG_VART(counts);

  if (counts.size() > 0)
  {
    langsStr = "Non-English language tag counts (sorted by reverse frequency):\n";

    for (QList<int>::const_iterator countsItr = counts.begin(); countsItr != counts.end();
         ++countsItr)
    {
      const int count = *countsItr;
      LOG_VART(count);

      QMap<int, QString>::const_iterator langNamesForCountItr = langCountsSwapped.constFind(count);
      while (langNamesForCountItr != langCountsSwapped.end() && langNamesForCountItr.key() == count)
      {
        const QString langName = *langNamesForCountItr;
        LOG_VART(langName);
        langsStr += langName + ": " + QString::number(count) + "\n";
        ++langNamesForCountItr;
      }
    }
    langsStr.chop(1);
  }

  return langsStr;
}

QString NonEnglishLanguageDetectionVisitor::getLangCountsSortedByLangName() const
{
  QString langsStr = "Non-English language tag counts (sorted by name):\n";
  for (QMap<QString, int>::const_iterator langsItr = _langNamesToCounts.begin();
       langsItr != _langNamesToCounts.end(); ++langsItr)
  {
    langsStr += langsItr.key() + ": " + QString::number(langsItr.value()) + "\n";
  }
  langsStr.chop(1);
  return langsStr;
}

void NonEnglishLanguageDetectionVisitor::visit(const boost::shared_ptr<Element>& e)
{
  if (_tagKeys.isEmpty())
  {
    throw HootException("No tag keys specified for language detection.");
  }

  LOG_VART(e);

  //if this var was set while parsing the previous element, increment the counter now
  if (_currentElementHasSuccessfulTagDetection)
  {
    _numElementsWithSuccessfulTagDetection++;
  }
  _currentElementHasSuccessfulTagDetection = false;

  const Tags& tags = e->getTags();
  bool elementProcessed = false;
  for (QSet<QString>::const_iterator tagKeysItr = _tagKeys.begin();
       tagKeysItr != _tagKeys.end(); ++tagKeysItr)
  {
    const QString tagKey = *tagKeysItr;
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
          {
            e->getTags().appendValue("hoot:detected:source:language:" + tagKey, langName);
          }

          if (_langNamesToCounts.contains(langName))
          {
            _langNamesToCounts[langName] = _langNamesToCounts[langName] + 1;
          }
          else
          {
            _langNamesToCounts[langName] = 1;
          }

          _numTagDetectionsMade++;
          _currentElementHasSuccessfulTagDetection = true;
          if (_numTagDetectionsMade % _taskStatusUpdateInterval == 0)
          {
            PROGRESS_INFO(
              _numTagDetectionsMade << " / " << _numProcessedTags << " tag language detections " <<
               "made on  " << _numProcessedElements << " / " << _numTotalElements << " elements.");
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
  {
    _numProcessedElements++;
//    if (_numProcessedElements % _taskStatusUpdateInterval == 0)
//    {
//      PROGRESS_INFO("Attempted language detection for " << _numProcessedElements << " elements.");
//    }
  }

  _numTotalElements++;
//  if (_numTotalElements % _taskStatusUpdateInterval == 0)
//  {
//    PROGRESS_INFO("Visited " << _numTotalElements << " elements.");
//  }
}

}
