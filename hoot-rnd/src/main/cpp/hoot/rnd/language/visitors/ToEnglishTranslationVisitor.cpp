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

#include "ToEnglishTranslationVisitor.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/algorithms/string/MostEnglishName.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ConstElementVisitor, ToEnglishTranslationVisitor)

ToEnglishTranslationVisitor::ToEnglishTranslationVisitor() :
_skipPreTranslatedTags(false),
_numTotalElements(0),
_skipWordsInEnglishDict(true),
_numTranslationsMade(0),
_numProcessedElements(0),
_numDetectionsMade(0),
_taskStatusUpdateInterval(10000)
{
}

ToEnglishTranslationVisitor::~ToEnglishTranslationVisitor()
{
  LOG_INFO("Language detections made: " << _numDetectionsMade);
  LOG_INFO("Successful tag translations made: " << _numTranslationsMade);
  LOG_INFO(
    "Attempted to translate tags for : " << _numProcessedElements << " elements out of " <<
    _numTotalElements << " elements encountered.");
}

void ToEnglishTranslationVisitor::setConfiguration(const Settings& conf)
{
  ConfigOptions opts(conf);

  _translatorClient.reset(
    Factory::getInstance().constructObject<ToEnglishTranslator>(
      opts.getLanguageTranslationTranslator()));
  boost::shared_ptr<QObject> qObj = boost::dynamic_pointer_cast<QObject>(_translatorClient);
  if (qObj.get())
  {
    LOG_DEBUG("Setting parent on translator client...");
    qObj->setParent(this);
  }
  _translatorClient->setConfiguration(conf);
  _translatorClient->setSourceLanguages(opts.getLanguageTranslationSourceLanguages());

  _toTranslateTagKeys = opts.getLanguageTranslationToTranslateTagKeys();
  _skipPreTranslatedTags = opts.getLanguageTranslationSkipPreTranslatedTags();
  _skipWordsInEnglishDict = opts.getLanguageTranslationSkipWordsInEnglishDictionary();
  _taskStatusUpdateInterval = opts.getTaskStatusUpdateInterval();
}

void ToEnglishTranslationVisitor::visit(const boost::shared_ptr<Element>& e)
{
  LOG_VART(e);
  const Tags& tags = e->getTags();
  for (int i = 0; i < _toTranslateTagKeys.size(); i++)
  {
    const QString toTranslateTagKey = _toTranslateTagKeys.at(i);
    if ((tags.contains(toTranslateTagKey)))
    {
      _translate(e, toTranslateTagKey);
    }
  }

  _numTotalElements++;
  if (_numTotalElements % _taskStatusUpdateInterval == 0)
  {
    PROGRESS_INFO("Visited " << _numTotalElements << " elements.");
  }
}

void ToEnglishTranslationVisitor::_translate(const ElementPtr& e,
                                             const QString toTranslateTagKey)
{
  const Tags& tags = e->getTags();
  _toTranslateTagKey = toTranslateTagKey;
  _element = e;
  _toTranslateVal = tags.get(toTranslateTagKey).trimmed();
  //We're just translating single phrases, not entire texts, so assume just one line was passed
  //in and remove the ending newline.
  //_toTranslateVal = _toTranslateVal.replace("\n", "");
  LOG_VART(_toTranslateVal);

  //making skipping tags that already have an english translated tag optional, b/c a many of the
  //OSM english translations I've seen are either just copies of the foreign language text or are
  //not very good translation
  const QString preTranslatedTagKey = _toTranslateTagKey + ":en";
  if (_skipPreTranslatedTags && tags.contains(preTranslatedTagKey))
  {
    LOG_DEBUG(
      "Skipping element with pre-translated tag: " << preTranslatedTagKey << "=" <<
      _toTranslateVal);
    return;
  }

  //This is an attempt to cut back on translation service requests for text that may already
  //in English.  Leaving it optional, b/c MostEnglishName's ability to determine if a word is
  //English is still a little suspect at this point.
  if (_skipWordsInEnglishDict)
  {
    const double englishNameScore = MostEnglishName::getInstance()->scoreName(_toTranslateVal);
    LOG_TRACE("English name score: " << englishNameScore << " for: " << _toTranslateVal);
    if (englishNameScore == 1.0)
    {
      LOG_DEBUG(
        "Tag value to be translated determined to already be in English.  Skipping " <<
        "translation; text: " << _toTranslateVal);
      return;
    }
  }

  _translatorClient->translate(_toTranslateVal);

  _numProcessedElements++;
  if (_numProcessedElements % 10 == 0)
  {
    PROGRESS_DEBUG("Processed " << _numProcessedElements << " elements.");
  }
}

void ToEnglishTranslationVisitor::translationComplete()
{
  const QString translatedVal = _translatorClient->getTranslatedText();
  LOG_VART(translatedVal);
  if (_translatorClient->detectionMade())
  {
    _numDetectionsMade++;
  }
  const int strComparison = translatedVal.compare(_toTranslateVal, Qt::CaseInsensitive);
  LOG_VART(strComparison);
  //If the translator merely returned the same string we passed in as the translated text, then
  //no point in using it.
  if (strComparison != 0)
  {
    LOG_DEBUG("Translated: " << _toTranslateVal << " to: " << translatedVal);
    _element->getTags().appendValue("hoot:translated:" + _toTranslateTagKey + ":en", translatedVal);
    _numTranslationsMade++;
    if (_numTranslationsMade % 10 == 0)
    {
      LOG_VARD(_numTranslationsMade);
    }
  }
  else
  {
    LOG_DEBUG(
      "Translator returned translation with same value as text passed in.  Discarding " <<
      "translation; text: " << translatedVal);
  }
}

void ToEnglishTranslationVisitor::translationError(QString textSent, QString message)
{
  LOG_ERROR("Error occurred during translation.  error: " + message + "; text sent: " + textSent);
}

}
